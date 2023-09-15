#include "test_common.hpp"
#include <catch2/catch_test_macros.hpp>

#include <InputSensor.hpp>
#include <OutputSensor.hpp>

// #include <server/Server.hpp>
#include <net/ServerSocket.hpp>

#include <utils/Utils.hpp>

#ifdef HUB_TESTS_MQTT_FOUND
#include <mqtt/client.h>
#endif

#include <Version.h>

TEST_CASE( "Server test : InputOutputSensor" ) {
    const auto hostname = hub::utils::getHostname();
    //std::cout << "[test] tested on machine: '" << hostname << "'" << std::endl;
    //return;

    constexpr int nAcqs       = 100;
    constexpr int width       = 1920;
    constexpr int height      = 1080;
    constexpr size_t dataSize = width * height * 3;
    //    unsigned char* data       = new unsigned char[dataSize];
    //    unsigned char* data2      = new unsigned char[dataSize];
    unsigned char* datas = new unsigned char[nAcqs * dataSize];
    //    unsigned char* data2 = new unsigned char[dataSize];

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

    //    std::vector<hub::Acquisition> acqs( nAcqs );
    //    for ( int i = 0; i < dataSize; ++i ) {
    //        data[i] = i % 256;
    //    }

    for ( int iAcq = 0; iAcq < nAcqs; ++iAcq ) {
        const unsigned char* data = &datas[iAcq * dataSize];
        hub::Acquisition acq( iAcq, iAcq );
        acq << hub::Measure( reinterpret_cast<unsigned const char*>( data ),
                                   dataSize,
                                   { { width, height }, hub::Format::BGR8 } );
        acqs.at( iAcq ) = std::move( acq );
    }


//    double megaBytesPerSeconds2;

    // raw socket
    double megaBytesPerSecondsClientSocket;
    {
        const std::string ipv4 = "127.0.0.1";
        const int port         = GET_RANDOM_PORT;
        // measure direct tcp stream duration
        std::cout << "[test][ClientSocket] start streaming" << std::endl;
        hub::net::ServerSocket serverSocket( port );
        hub::net::ClientSocket clientSocket( ipv4, port );
        auto clientServerSocket = serverSocket.waitNewClient();
#ifdef OS_MACOS
        const int packetSize    = 2'000'000; // 2Go network memory buffer Linux, Win
#else
        const int packetSize    = 400'000; // 400Mo network memory buffer MacOS, Mac Mini M2
#endif
        const int nPart         = dataSize / packetSize;
	std::cout << "[test][ClientSocket] nPart: " << nPart << " of " << packetSize / 1000.0 << " Mo" << std::endl;

        unsigned char* dataIn = new unsigned char[dataSize];

        const auto& start = std::chrono::high_resolution_clock::now();
        {
            for ( int iAcq = 0; iAcq < nAcqs; ++iAcq ) {
#ifdef OS_MACOS
        std::cout << "[test][ClientSocket] send acq " << iAcq << std::endl;
#endif
                const unsigned char* data = &datas[iAcq * dataSize];
                int uploadSize            = 0;

                for ( int i = 0; i < nPart - 1; ++i ) {
#ifdef OS_MACOS
        std::cout << "[test][ClientSocket] sending part " << i << std::endl;
#endif
                    clientSocket.write( data + uploadSize, packetSize );
	 	//std::cout << "[test][ClientSocket] part sended " << i << std::endl;

#ifdef OS_MACOS
        std::cout << "[test][ClientSocket] reading part " << i << std::endl;
#endif
                    clientServerSocket.read( dataIn + uploadSize, packetSize );
	 	//std::cout << "[test][ClientSocket] part readed " << i << std::endl;

                    uploadSize += packetSize;
                }

                clientSocket.write( data + uploadSize, dataSize - uploadSize );
                clientServerSocket.read( dataIn + uploadSize, dataSize - uploadSize );

                assert( !memcmp( data, dataIn, dataSize ) );
            }
        }
        const auto& end = std::chrono::high_resolution_clock::now();
        std::cout << "[test][ClientSocket] end streaming" << std::endl;
        const auto& duration =
            std::chrono::duration_cast<std::chrono::milliseconds>( end - start ).count();
        const auto& bytes               = dataSize * nAcqs;
        const auto& bytesPerSeconds     = 1000.0 * bytes / duration;
        megaBytesPerSecondsClientSocket = bytesPerSeconds / 1000'000.0;

        std::cout << "[test][ClientSocket] Mega byte wrote : " << bytes / 1000'000.0 << " Mo"
                  << std::endl;
        std::cout << "[test][ClientSocket] Mega byte per second : "
                  << megaBytesPerSecondsClientSocket << " Mo/s" << std::endl;
    }

#ifdef HUB_TESTS_MQTT_FOUND
    // raw mqtt
    double megaBytesPerSecondsMqtt;
    {
        //        const std::string topicName = "sensor";
        const std::string topicName = FILE_NAME;
        std::cout << "[test][Mqtt] start streaming" << std::endl;
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
        outputMsgPtr->set_qos( 0 );

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
            const auto& payload = inputMsgPtr->get_payload();
            const auto* data3   = payload.data();
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
        std::cout << "[test][Mqtt] end streaming" << std::endl;
        const auto& duration2 =
            std::chrono::duration_cast<std::chrono::milliseconds>( end2 - start2 ).count();
        const auto& bytes            = dataSize * nAcqs;
        const auto& bytesPerSeconds2 = 1000.0 * bytes / duration2;
        megaBytesPerSecondsMqtt      = bytesPerSeconds2 / 1000'000.0;

        std::cout << "[test][Mqtt] Mega byte wrote : " << bytes / 1000'000.0 << " Mo" << std::endl;
        std::cout << "[test][Mqtt] Mega byte per second : " << megaBytesPerSecondsMqtt << " Mo/s"
                  << std::endl;
    }
#endif

    enum class Implement {
        SERVER = 0,
#ifdef HUB_BUILD_MQTT
        MQTT,
#endif
        COUNT,
    };
    static std::string implements2string[static_cast<int>( Implement::COUNT )] = { "Server",
#ifdef HUB_BUILD_MQTT
                                                                                   "Mqtt"
#endif
    };
    double megaBytesPerSecondsImpls[(int)Implement::COUNT] = {0};

    //    for (const auto & implement : implements)
    for ( int i = 0; i < (int)Implement::COUNT; ++i ) {
        const auto implement     = Implement( i );
        const auto implementStr  = implements2string[(int)implement];
        const std::string header = "[test][" + implementStr + "Impl] ";

        std::cout << header << "############################### server start" << std::endl;
        std::cout << header << "server end ------------------------------" << std::endl;

        {
            std::cout << header << "############################### outputStream start"
                      << std::endl;

            std::unique_ptr<hub::OutputSensor> outputSensor;

            switch ( implement ) {
            case Implement::SERVER:
                outputSensor = std::make_unique<hub::OutputSensor>(
                    hub::SensorSpec( "sensorName", { { { width, height }, hub::Format::BGR8 } } ),
                    hub::output::OutputStreamServer( FILE_NAME ) );
                break;
#ifdef HUB_BUILD_MQTT
            case Implement::MQTT:
                outputSensor = std::make_unique<hub::OutputSensor>(
                    hub::SensorSpec( "sensorName", { { { width, height }, hub::Format::BGR8 } } ),
                    hub::output::OutputStreamMqtt( FILE_NAME ) );
                break;
#endif
            default:
                assert( false );
                break;
            }

            //            hub::OutputSensor outputSensor(
            //                hub::SensorSpec( "sensorName", { { { width, height },
            //                hub::Format::BGR8 } } ), OutputStream( FILE_NAME )
            //            );

            std::cout
                << "[test][InputOutputSensor] ############################### inputStream start"
                << std::endl;
            std::unique_ptr<hub::InputSensor> inputSensor;

            switch ( implement ) {
            case Implement::SERVER:
                inputSensor =
                    std::make_unique<hub::InputSensor>( hub::input::InputStreamServer( FILE_NAME ) );
                break;
#ifdef HUB_BUILD_MQTT
            case Implement::MQTT:
                inputSensor =
                    std::make_unique<hub::InputSensor>( hub::input::InputStreamMqtt( FILE_NAME ) );
                break;
#endif
            default:
                assert( false );
                break;
            }

            //            hub::InputSensor inputSensor(
            //                hub::input::InputStream( FILE_NAME ) );

            const auto& inputSensorSpec = inputSensor->getSpec();
            CHECK( inputSensorSpec.getAcquisitionSize() == dataSize );
            CHECK( inputSensorSpec.getSensorName() == "sensorName" );

            const auto& resolutions = inputSensorSpec.getResolutions();
            CHECK( resolutions.size() == 1 );
            CHECK( resolutions[0].first.size() == 2 );
            CHECK( resolutions[0].first.at( 0 ) == width );
            CHECK( resolutions[0].first.at( 1 ) == height );
            CHECK( resolutions[0].second == hub::Format::BGR8 );
            std::cout << header << "inputStream end ---------------------------------" << std::endl;

            std::cout << header << "############################### send acquisitions" << std::endl;
            const auto& start2 = std::chrono::high_resolution_clock::now();
            for ( int i = 0; i < nAcqs; ++i ) {
                *outputSensor << acqs.at( i );
                hub::Acquisition acq;
                *inputSensor >> acq;
                CHECK( acq == acqs.at( i ) );
            }
            const auto& end2 = std::chrono::high_resolution_clock::now();
            const auto& duration2 =
                std::chrono::duration_cast<std::chrono::milliseconds>( end2 - start2 ).count();
            const auto& bytes2           = dataSize * nAcqs;
            const auto& bytesPerSeconds2 = 1000.0 * bytes2 / duration2;
            megaBytesPerSecondsImpls[(int)implement]         = bytesPerSeconds2 / 1000'000.0;

            std::cout << header << "Mega byte wrote : " << bytes2 / 1000'000.0 << " Mo"
                      << std::endl;
            std::cout << header << "Mega byte per second : " << megaBytesPerSecondsImpls[(int)implement] << " Mo/s"
                      << std::endl;
        }

        // #ifdef WIN32
        // #    ifdef DEBUG
        //         checkRatio( ratio, 20 );
        // #    else
        //         checkRatio( ratio, 40 );
        // #    endif
        // #else
        //         if ( hostname == "msi" ) { checkRatio( ratio, 50, 15 ); }
        //         else {
        // #ifdef DEBUG
        //             checkRatio( ratio, 50, 10 );
        // #else
        //             checkRatio( ratio, 40, 10 );
        // #endif
        //         }
        // #endif
    }

    std::cout << std::endl;

    std::cout << "######################################## RESUME ########################################" << std::endl;
    std::cout << std::endl;

    std::cout << "[test][ClientSocket] Mega byte per second : " << megaBytesPerSecondsClientSocket
              << " Mo/s" << std::endl;
#ifdef HUB_TESTS_MQTT_FOUND
    std::cout << "[test][Mqtt] Mega byte per second : " << megaBytesPerSecondsMqtt << " Mo/s"
              << std::endl;
#endif

    for ( int i = 0; i < (int)Implement::COUNT; ++i ) {
        const auto implement     = Implement( i );
        const auto implementStr  = implements2string[(int)implement];
        const std::string header = "[test][" + implementStr + "Impl] ";
        std::cout << header << "Mega byte per second : " << megaBytesPerSecondsImpls[(int)implement] << " Mo/s"
              << std::endl;
    }


    //    std::cout << "[test][InputOutputSensor] Mega byte per second : " << megaBytesPerSeconds2
    //              << " Mo/s" << std::endl;

    std::cout << std::endl;

    double megaBytesPerSecondsServerImpl = megaBytesPerSecondsImpls[(int)Implement::SERVER];
#ifdef HUB_BUILD_MQTT
    double megaBytesPerSecondsMqttImpl = megaBytesPerSecondsImpls[(int)Implement::MQTT];
#endif

    double ratio;
#ifdef HUB_BUILD_SERVER
    // server performance with viewer and nativeViewer running
    ratio = 100.0 * megaBytesPerSecondsServerImpl / megaBytesPerSecondsClientSocket;
//    std::cout << "[Hub/ClientSocket] ratio : " << ratio << " %" << std::endl;
//    checkRatio( ratio, 50, 50, "Hub/ClientSocket" );
     if ( hostname == "asus-b450" ) // linux CI runner
    {
#ifdef DEBUG
        checkRatio( ratio, 35, 5, "Hub/ClientSocket" );
#else
        checkRatio( ratio, 25, 5 , "Hub/ClientSocket");
#endif
     }
     else if (hostname == "gigabyte-Z370P") { // windows CI runner
#ifdef DEBUG
        checkRatio( ratio, 10, 5, "Hub/ClientSocket" );
#else
        checkRatio( ratio, 10, 5, "Hub/ClientSocket" );
#endif
     }
     else if (hostname == "Mac-mini-de-gauthier.local") { // macOs CI runner
#ifdef DEBUG
        checkRatio( ratio, 40, 5, "Hub/ClientSocket" );
#else
        checkRatio( ratio, 80, 5, "Hub/ClientSocket" );
#endif
     }
     else {
        checkRatio( ratio, 50, 50, "Hub/ClientSocket" );
     }

#endif

#ifdef HUB_TESTS_MQTT_FOUND
    ratio = 100.0 * megaBytesPerSecondsMqtt / megaBytesPerSecondsClientSocket;
//    std::cout << "[Mqtt/ClientSocket] ratio : " << ratio << " %" << std::endl;
    checkRatio(ratio, 12, 12, "Mqtt/ClientSocket");
#endif


#ifdef HUB_BUILD_MQTT
    ratio = 100.0 * megaBytesPerSecondsMqttImpl / megaBytesPerSecondsClientSocket;
//    std::cout << "[MqttStream/ClientSocket] ratio : " << ratio << " %" << std::endl;
    checkRatio(ratio, 4, 4, "MqttImpl/ClientSocket");

    ratio = 100.0 * megaBytesPerSecondsMqttImpl / megaBytesPerSecondsMqtt;
    std::cout << "[MqttImpl/Mqtt] ratio : " << ratio << " %" << std::endl;
//    checkRatio(ratio, 50, 15);
#endif

    std::cout << std::endl;

#ifdef HUB_BUILD_SERVER
#ifdef HUB_TESTS_MQTT_FOUND
    ratio = 100.0 * megaBytesPerSecondsMqtt / megaBytesPerSecondsServerImpl;
    std::cout << "[Mqtt/Hub] ratio : " << ratio << " %" << std::endl;
#endif
//    checkRatio(ratio, 5, 5);

#ifdef HUB_BUILD_MQTT
    ratio = 100.0 * megaBytesPerSecondsMqttImpl / megaBytesPerSecondsServerImpl;
    std::cout << "[MqttImpl/Hub] ratio : " << ratio << " %" << std::endl;
//    checkRatio(ratio, 12, 5);
#endif

    std::cout << std::endl;

#ifdef HUB_TESTS_MQTT_FOUND
    std::cout << "Hub server is " << megaBytesPerSecondsImpls[(int)Implement::SERVER] / megaBytesPerSecondsMqtt << " more efficient than raw mqtt stream (Qos = 0 -> can lost packet)" << std::endl;
#endif
#ifdef HUB_BUILD_MQTT
    std::cout << "Hub server is " << megaBytesPerSecondsImpls[(int)Implement::SERVER] / megaBytesPerSecondsImpls[(int)Implement::MQTT] << " more efficient than mqtt implement (Qos = 2 -> no loss)" << std::endl;
#endif
#endif

    std::cout << std::endl;

    //    const auto ratio = 100.0 * megaBytesPerSeconds2 / megaBytesPerSeconds;
    //    std::cout << "[test][ClientSocket/InputOutputSensor] ratio : " << ratio << " %" <<
    //    std::endl;

    std::cout << "[test] tested on machine: '" << hostname << "'" << std::endl;

    delete[] datas;
    //    delete[] data2;
}
