
 #include <catch2/catch_test_macros.hpp>

 #include "test-common.hpp"

 #include <InputSensor.hpp>
 #include <OutputSensor.hpp>

 #include <server/Server.hpp>
 // #include <net/ServerSocket.hpp>

 #include <filesystem>

 #include <mqtt/client.h>
 //#include <mqtt/client.hpp>

 TEST_CASE( "MqttCompareSpeed test" ) {

     // int main() {
     constexpr int nAcqs       = 60;
     constexpr int width       = 1920;
     constexpr int height      = 1080;
     constexpr size_t dataSize = width * height * 3;
     unsigned char* data       = new unsigned char[dataSize];
     unsigned char* data2      = new unsigned char[dataSize];

     std::vector<hub::Acquisition> acqs( nAcqs );
     for ( int i = 0; i < dataSize; ++i ) {
         data[i] = i % 256;
     }

     for ( int iAcq = 0; iAcq < nAcqs; ++iAcq ) {
         hub::Acquisition acq( iAcq, iAcq );
         acq << hub::data::Measure( reinterpret_cast<unsigned const char*>( data ),
                                    dataSize,
                                    { { width, height }, hub::Format::BGR8 } );
         acqs.at( iAcq ) = std::move( acq );
     }

     const std::string ipv4 = "127.0.0.1";
     const int port         = GET_RANDOM_PORT;

     double megaBytesPerSeconds;
     double megaBytesPerSeconds2;
     double megaBytesPerSeconds3;

     {
         // measure direct tcp stream duration
         std::cout << "[test][ClientSocket] start streaming" << std::endl;
         hub::net::ServerSocket serverSocket( port );
         hub::net::ClientSocket clientSocket( ipv4, port );
         auto clientServerSocket = serverSocket.waitNewClient();
         const int packetSize    = 2'000'000; // 2Go network memory buffer
         const int nPart         = dataSize / packetSize;
         const auto& start       = std::chrono::high_resolution_clock::now();
         {
             for ( int iAcq = 0; iAcq < nAcqs; ++iAcq ) {
                 int uploadSize = 0;

                 for ( int i = 0; i < nPart - 1; ++i ) {
                     clientSocket.write( data + uploadSize, packetSize );
                     clientServerSocket.read( data2 + uploadSize, packetSize );

                     uploadSize += packetSize;
                 }

                 clientSocket.write( data + uploadSize, dataSize - uploadSize );
                 clientServerSocket.read( data2 + uploadSize, dataSize - uploadSize );

                 CHECK( !memcmp( data, data2, dataSize ) );
             }
         }
         const auto& end = std::chrono::high_resolution_clock::now();
         std::cout << "[test][ClientSocket] end streaming" << std::endl;
         const auto& duration =
             std::chrono::duration_cast<std::chrono::milliseconds>( end - start ).count();
         const auto& bytes           = dataSize * nAcqs;
         const auto& bytesPerSeconds = 1000.0 * bytes / duration;
         megaBytesPerSeconds         = bytesPerSeconds / 1000'000.0;

         std::cout << "[test][ClientSocket] Mega byte wrote : " << bytes / 1000'000.0 << " Mo"
                   << std::endl;
         std::cout << "[test][ClientSocket] Mega byte per second : " << megaBytesPerSeconds
                   << " Mo/s" << std::endl;
         std::cout << "*****************************************************************************"
                   << std::endl;
     }

     {
         std::cout << "[Mqtt] start streaming" << std::endl;
         const std::string ip = "localhost:1883";
         mqtt::client inputClient( ip, "consumer", mqtt::create_options( MQTTVERSION_5 ) );
         inputClient.connect();
         inputClient.subscribe( "sensor" );
         inputClient.start_consuming();

         //            hub::net::ClientSocket clientSocket( ipv4, port );
         //            auto clientServerSocket = serverSocket.waitNewClient();
         mqtt::client outputClient( ip, "producer", mqtt::create_options( MQTTVERSION_5 ) );
         outputClient.connect();
         const mqtt::message_ptr outputMsgPtr = mqtt::make_message( "sensor", "" );

         mqtt::const_message_ptr inputMsgPtr;
         const auto& start = std::chrono::high_resolution_clock::now();
         //            hub::net::ServerSocket serverSocket( port );

         //            const int packetSize    = 2'000'000; // 2Go network memory buffer
         //            const int nPart         = dataSize / packetSize;
         for ( int iAcq = 0; iAcq < nAcqs; ++iAcq ) {
             //                int uploadSize = 0;

             outputMsgPtr->set_payload( data, dataSize );
             outputClient.publish( outputMsgPtr );

             inputMsgPtr = inputClient.consume_message();

             const auto& payload  = inputMsgPtr->get_payload_ref();
             const auto* data3    = payload.data();
             const auto data3Size = payload.size();

             //                for ( int i = 0; i < nPart - 1; ++i ) {
             //                    clientSocket.write( data + uploadSize, packetSize );
             //                    clientServerSocket.read( data2 + uploadSize, packetSize );

             //                    uploadSize += packetSize;
             //                }

             //                clientSocket.write( data + uploadSize, dataSize - uploadSize );
             //                clientServerSocket.read( data2 + uploadSize, dataSize - uploadSize
             //                );

             //                assert( !memcmp( data, data2, dataSize ) );
             CHECK( dataSize == data3Size );
             CHECK( !memcmp( data, data3, dataSize ) );
         }
         inputClient.stop_consuming();
         const auto& end = std::chrono::high_resolution_clock::now();
         std::cout << "[Mqtt] end streaming" << std::endl;
         const auto& duration =
             std::chrono::duration_cast<std::chrono::milliseconds>( end - start ).count();
         const auto& bytes           = dataSize * nAcqs;
         const auto& bytesPerSeconds = 1000.0 * bytes / duration;
         megaBytesPerSeconds2        = bytesPerSeconds / 1000'000.0;

         std::cout << "[Mqtt] Mega byte wrote : " << bytes / 1000'000.0 << " Mo" << std::endl;
         std::cout << "[Mqtt] Mega byte per second : " << megaBytesPerSeconds2 << " Mo/s"
                   << std::endl;
         const auto ratio = 100.0 * megaBytesPerSeconds2 / megaBytesPerSeconds;
         std::cout << "[Mqtt] ratio : " << ratio << " %" << std::endl;
         std::cout << "*****************************************************************************"
                   << std::endl;
     }

     {
         const int port2 = port + 1;

         std::cout << "[test][InputOutputSensor] ############################### server start"
                   << std::endl;
         hub::server::Server server( port2 );
         server.setMaxClients( 2 );
         server.asyncRun();
         std::cout << "[test][InputOutputSensor] server end ------------------------------"
                   << std::endl;

         {
             std::cout
                 << "[test][InputOutputSensor] ############################### outputStream start"
                 << std::endl;
             hub::OutputSensor outputSensor(
                 hub::SensorSpec( "sensorName", { { { width, height }, hub::Format::BGR8 } } ),
                 "streamName",
                 hub::net::ClientSocket( ipv4, port2 ) );

             std::cout
                 << "[test][InputOutputSensor] ############################### inputStream start"
                 << std::endl;
             hub::InputSensor inputSensor(
                 hub::io::InputStream( "streamName", hub::net::ClientSocket( ipv4, port2 ) ) );

             const auto& inputSensorSpec = inputSensor.getSpec();
             CHECK( inputSensorSpec.getAcquisitionSize() == dataSize );
             CHECK( inputSensorSpec.getSensorName() == "sensorName" );

             const auto& resolutions = inputSensorSpec.getResolutions();
             CHECK( resolutions.size() == 1 );
             CHECK( resolutions[0].first.size() == 2 );
             CHECK( resolutions[0].first.at( 0 ) == width );
             CHECK( resolutions[0].first.at( 1 ) == height );
             CHECK( resolutions[0].second == hub::Format::BGR8 );
             std::cout
                 << "[test][InputOutputSensor] inputStream end ---------------------------------"
                 << std::endl;

             std::cout
                 << "[test][InputOutputSensor] ############################### send acquisitions"
                 << std::endl;
             const auto& start3 = std::chrono::high_resolution_clock::now();
             for ( int i = 0; i < nAcqs; ++i ) {
                 outputSensor << acqs.at( i );
                 hub::Acquisition acq;
                 inputSensor >> acq;
                 CHECK( acq == acqs.at( i ) );
             }
             const auto& end3 = std::chrono::high_resolution_clock::now();
             const auto& duration3 =
                 std::chrono::duration_cast<std::chrono::milliseconds>( end3 - start3 ).count();
             const auto& bytes3           = dataSize * nAcqs;
             const auto& bytesPerSeconds3 = 1000.0 * bytes3 / duration3;
             megaBytesPerSeconds3         = bytesPerSeconds3 / 1000'000.0;

             std::cout << "[test][InputOutputSensor] Mega byte wrote : " << bytes3 / 1000'000.0
                       << " Mo" << std::endl;
             std::cout << "[test][InputOutputSensor] Mega byte per second : " << megaBytesPerSeconds3
                       << " Mo/s" << std::endl;

             const auto ratio = 100.0 * megaBytesPerSeconds3 / megaBytesPerSeconds;
             std::cout << "[test][ClientSocket/InputOutputSensor] ratio : " << ratio << " %"
                       << std::endl;

 #ifdef WIN32
 #    ifdef DEBUG
             CHECK( ratio > 15 );
 #    else
             CHECK( ratio > 35 );
 #    endif
 #else
             CHECK( ratio > 35 );
 #endif
         }

         const auto ratioMqttHub = 100.0 * megaBytesPerSeconds2 / megaBytesPerSeconds3;
         std::cout << "[Mqtt/Hub] ratio : " << ratioMqttHub << " %" << std::endl;
         CHECK( ratioMqttHub < 50 );
     }

     delete[] data;
     delete[] data2;
 }
