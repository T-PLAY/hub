
#include <catch2/catch_test_macros.hpp>

#include "test_common.hpp"

#include <InputSensor.hpp>
#include <OutputSensor.hpp>

//#include <server/Server.hpp>
#include <net/ServerSocket.hpp>

#include <filesystem>

#include <mqtt/client.h>
// #include <mqtt/client.hpp>

TEST_CASE( "MqttCompareSpeed test" ) {

    // int main() {
    constexpr int nAcqs       = 30;
    constexpr int width       = 1920;
    constexpr int height      = 1080;
    constexpr size_t dataSize = width * height * 3;
    //     unsigned char * datas[nAcqs][dataSize]       = new unsigned char[nAcqs][dataSize];
    //     unsigned char * datas[nAcqs][dataSize]       = new unsigned char[nAcqs][dataSize];
    unsigned char* datas = new unsigned char[nAcqs * dataSize];
    //     unsigned char * data[nAcqs];
    //     data = (unsigned char* [nAcqs])&linearized;
    //     unsigned char * datas[nAcqs] = reinterpret_cast<unsigned char[nAcqs]>(linearized);
    //     datas = (unsigned char[nAcqs])linearized;
    unsigned char* data2 = new unsigned char[dataSize];

    srand( (unsigned)time( NULL ) );
    //    int port = rand() % 60000;
    std::vector<hub::Acquisition> acqs( nAcqs );
    for ( int i = 0; i < nAcqs; ++i ) {
        for ( int j = 0; j < dataSize; ++j ) {
            //         datas[i] = i % 256;
            //            datas[i * dataSize + j] = rand() % 256;
            datas[i * dataSize + j] = 65 + i % 256;
        }
    }

    for ( int iAcq = 0; iAcq < nAcqs; ++iAcq ) {
        hub::Acquisition acq( iAcq, iAcq );
        acq << hub::data::Measure(
            reinterpret_cast<unsigned const char*>( &datas[iAcq * dataSize] ),
            dataSize,
            { { width, height }, hub::Format::BGR8 } );
        acqs.at( iAcq ) = std::move( acq );
    }

    const std::string ipv4 = "127.0.0.1";
    const int port         = GET_RANDOM_PORT;
//    const int port = hub::s_serverDefaultPort;

    double megaBytesPerSeconds;
    double megaBytesPerSeconds2;
#ifdef BUILD_SERVER
    double megaBytesPerSeconds3;
#endif
    double megaBytesPerSeconds4;

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
                    clientSocket.write( &datas[iAcq * dataSize] + uploadSize, packetSize );
                    clientServerSocket.read( data2 + uploadSize, packetSize );

                    uploadSize += packetSize;
                }

                clientSocket.write( &datas[iAcq * dataSize] + uploadSize, dataSize - uploadSize );
                clientServerSocket.read( data2 + uploadSize, dataSize - uploadSize );

                CHECK( !memcmp( &datas[iAcq * dataSize], data2, dataSize ) );
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
    }

    {
//        const std::string topicName = "sensor";
        const std::string topicName = __FILE_NAME__;
        std::cout << "[Mqtt] start streaming" << std::endl;
        const std::string ip = "localhost:1883";
        mqtt::client inputClient( ip, "consumer", mqtt::create_options( MQTTVERSION_5 ) );
        inputClient.connect();
        inputClient.subscribe( topicName );
        inputClient.start_consuming();

        //            hub::net::ClientSocket clientSocket( ipv4, port );
        //            auto clientServerSocket = serverSocket.waitNewClient();
        mqtt::client outputClient( ip, "producer", mqtt::create_options( MQTTVERSION_5 ) );
        outputClient.connect();
        const mqtt::message_ptr outputMsgPtr = mqtt::make_message( topicName, "" );
        //        outputMsgPtr->set_retained(true);
//                outputMsgPtr->set_qos(2);
                outputMsgPtr->set_qos(0);


        mqtt::const_message_ptr inputMsgPtr;
        const auto& start2 = std::chrono::high_resolution_clock::now();
        //            hub::net::ServerSocket serverSocket( port );

        //            const int packetSize    = 2'000'000; // 2Go network memory buffer
        //            const int nPart         = dataSize / packetSize;
        for ( int iAcq = 0; iAcq < nAcqs; ++iAcq ) {
//                            int uploadSize = 0;

            outputMsgPtr->set_payload( &datas[iAcq * dataSize], dataSize );
            //            outputMsgPtr->set_payload( datas, dataSize );
            outputClient.publish( outputMsgPtr );

            inputMsgPtr = inputClient.consume_message();
//            assert( inputMsgPtr != nullptr );

            //            const auto& payload  = inputMsgPtr->get_payload_ref();
            const auto& payload  = inputMsgPtr->get_payload();
            const auto* data3    = payload.data();
//            const auto data3Size = payload.size();

            //                for ( int i = 0; i < nPart - 1; ++i ) {
            //                    clientSocket.write( data + uploadSize, packetSize );
            //                    clientServerSocket.read( data2 + uploadSize, packetSize );

            //                    uploadSize += packetSize;
            //                }

            //                clientSocket.write( data + uploadSize, dataSize - uploadSize );
            //                clientServerSocket.read( data2 + uploadSize, dataSize - uploadSize
            //                );

            //                assert( !memcmp( data, data2, dataSize ) );
//            CHECK( dataSize == data3Size );
            CHECK( !memcmp( &datas[iAcq * dataSize], data3, dataSize ) );
//            assert( !memcmp( &datas[iAcq * dataSize], data3, dataSize ) );
        }
        inputClient.stop_consuming();
        const auto& end2 = std::chrono::high_resolution_clock::now();
        std::cout << "[Mqtt] end streaming" << std::endl;
        const auto& duration2 =
            std::chrono::duration_cast<std::chrono::milliseconds>( end2 - start2 ).count();
        const auto& bytes           = dataSize * nAcqs;
        const auto& bytesPerSeconds2 = 1000.0 * bytes / duration2;
        megaBytesPerSeconds2        = bytesPerSeconds2 / 1000'000.0;

        std::cout << "[Mqtt] Mega byte wrote : " << bytes / 1000'000.0 << " Mo" << std::endl;
    }

#ifdef BUILD_SERVER
    // hub server
    {
//        const int port2 = port + 1;

        std::cout << "[test][InputOutputSensor] ############################### server start"
                  << std::endl;
//        hub::Server server( port2 );
//        server.setMaxClients( 2 );
//        server.asyncRun();
        std::cout << "[test][InputOutputSensor] server end ------------------------------"
                  << std::endl;

        {
            std::cout
                << "[test][InputOutputSensor] ############################### outputStream start"
                << std::endl;
            hub::OutputSensor outputSensor(
                hub::SensorSpec( "sensorName", { { { width, height }, hub::Format::BGR8 } } ),
                OutputStreamServer(__FILE_NAME__));
                //                 hub::net::ClientSocket( ipv4, port2 ) );
//                ipv4,
//                port );

            std::cout
                << "[test][InputOutputSensor] ############################### inputStream start"
                << std::endl;
            hub::InputSensor inputSensor(
                //                 port2 ) ) );
                InputStreamServer( __FILE_NAME__ ) );

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
        }
    }
#endif

    {
//        const std::string ipv4 = "127.0.0.1";
//        const int port         = 1883;

        std::cout << "[test][MqttStream] ############################### outputStream start"
                  << std::endl;
        hub::OutputSensor outputSensor(
            hub::SensorSpec( "sensorName", { { { width, height }, hub::Format::BGR8 } } ),
            OutputStreamMqtt( __FILE_NAME__ ) );

        std::cout << "[test][MqttStream] ############################### inputStream start"
                  << std::endl;
        hub::InputSensor inputSensor( InputStreamMqtt( __FILE_NAME__ ) );


        const auto& inputSensorSpec = inputSensor.getSpec();
        CHECK( inputSensorSpec.getAcquisitionSize() == dataSize );
        CHECK( inputSensorSpec.getSensorName() == "sensorName" );

        const auto& resolutions = inputSensorSpec.getResolutions();
        CHECK( resolutions.size() == 1 );
        CHECK( resolutions[0].first.size() == 2 );
        CHECK( resolutions[0].first.at( 0 ) == width );
        CHECK( resolutions[0].first.at( 1 ) == height );
        CHECK( resolutions[0].second == hub::Format::BGR8 );
        std::cout << "[test][MqttStream] inputStream end ---------------------------------"
                  << std::endl;

        std::cout << "[test][MqttStream] ############################### send acquisitions"
                  << std::endl;
        const auto& start4 = std::chrono::high_resolution_clock::now();
        for ( int i = 0; i < nAcqs; ++i ) {
            outputSensor << acqs.at( i );
            hub::Acquisition acq;
            inputSensor >> acq;
            CHECK( acq == acqs.at( i ) );
        }
        const auto& end4 = std::chrono::high_resolution_clock::now();
        const auto& duration4 =
            std::chrono::duration_cast<std::chrono::milliseconds>( end4 - start4 ).count();
        const auto& bytes4           = dataSize * nAcqs;
        const auto& bytesPerSeconds4 = 1000.0 * bytes4 / duration4;
        megaBytesPerSeconds4         = bytesPerSeconds4 / 1000'000.0;

        std::cout << "[test][MqttStream] Mega byte wrote : " << bytes4 / 1000'000.0 << " Mo"
                  << std::endl;
//        std::cout << "[test][MqttStream] Mega byte per second : " << megaBytesPerSeconds4 << " Mo/s"
//                  << std::endl;

        // #ifdef WIN32
        // #    ifdef DEBUG
        //         CHECK( ratio > 15 );
        // #    else
        //         CHECK( ratio > 35 );
        // #    endif
        // #else
        //         CHECK( ratio > 35 );
        // #endif
    }

    std::cout << std::endl;
    std::cout << std::endl;

    std::cout << "[ClientSocket] Mega byte per second : " << megaBytesPerSeconds << " Mo/s"
              << std::endl;

#ifdef BUILD_SERVER
    std::cout << "[Hub] Mega byte per second : " << megaBytesPerSeconds3
              << " Mo/s" << std::endl;
#endif

    std::cout << "[Mqtt] Mega byte per second : " << megaBytesPerSeconds2 << " Mo/s" << std::endl;

    std::cout << "[MqttStream] Mega byte per second : " << megaBytesPerSeconds4 << " Mo/s" << std::endl;

    std::cout << std::endl;


//#ifdef WIN32
//#    ifdef DEBUG
//    //            CHECK( ratio > 15 );
//    checkRatio( ratio, 25 );
//#    else
//    //            CHECK( ratio > 35 );
//    checkRatio( ratio, 45 );
//#    endif
//#else
    //            CHECK( ratio > 35 );
//    checkRatio( ratio, 45 );
//#endif
    double ratio;
#ifdef BUILD_SERVER
    ratio = 100.0 * megaBytesPerSeconds3 / megaBytesPerSeconds;
    std::cout << "[Hub/ClientSocket] ratio : " << ratio << " %" << std::endl;
    checkRatio(ratio, 50, 15);
#endif

    ratio = 100.0 * megaBytesPerSeconds2 / megaBytesPerSeconds;
    std::cout << "[Mqtt/ClientSocket] ratio : " << ratio << " %" << std::endl;
    checkRatio(ratio, 12, 12);


    ratio = 100.0 * megaBytesPerSeconds4 / megaBytesPerSeconds;
    std::cout << "[MqttStream/ClientSocket] ratio : " << ratio << " %" << std::endl;
    checkRatio(ratio, 4, 4);

    ratio = 100.0 * megaBytesPerSeconds4 / megaBytesPerSeconds2;
    std::cout << "[MqttStream/Mqtt] ratio : " << ratio << " %" << std::endl;
//    checkRatio(ratio, 50, 15);

#ifdef BUILD_SERVER
    ratio = 100.0 * megaBytesPerSeconds2 / megaBytesPerSeconds3;
    std::cout << "[Mqtt/Hub] ratio : " << ratio << " %" << std::endl;
//    checkRatio(ratio, 5, 5);

    ratio = 100.0 * megaBytesPerSeconds4 / megaBytesPerSeconds3;
    std::cout << "[MqttStream/Hub] ratio : " << ratio << " %" << std::endl;
    checkRatio(ratio, 5, 5);

    std::cout << std::endl;

    std::cout << "Hub server is " << megaBytesPerSeconds3 / megaBytesPerSeconds2 << " more efficient than raw mqtt stream (Qos = 0 -> can lost packet)" << std::endl;
    std::cout << "Hub server is " << megaBytesPerSeconds3 / megaBytesPerSeconds4 << " more efficient than mqtt implement (Qos = 2 -> no loss)" << std::endl;
#endif

    delete[] datas;
    delete[] data2;
}
