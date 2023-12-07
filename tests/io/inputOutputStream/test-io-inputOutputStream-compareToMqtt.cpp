
#include "test_common.hpp"
#include "io/test_io_common.hpp"

// #include <server/Server.hpp>
#include <net/ServerSocket.hpp>

#include <io/input/InputStream.hpp>
#include <io/output/OutputStream.hpp>

#ifdef HUB_USE_MQTT
#    include <mqtt/client.h>
#endif

TEST_CASE( "Server test : InputOutputStream_vs_Mqtt" ) {
    //    const auto hostname = hub::utils::getHostname();

    // raw socket
    // double gigaBytePerSecondSocket;
    // {
    //     const std::string ipv4 = "127.0.0.1";
    //     const int port         = GET_RANDOM_PORT;
    //     // std::cout << "[test][ClientSocket] start streaming" << std::endl;
    //     hub::net::ServerSocket serverSocket( port );
    //     hub::net::ClientSocket clientSocket( ipv4, port );
    //     auto clientServerSocket = serverSocket.waitNewClient();

    //     const auto& [durationInMillisecond, gigaBytePerSecond] =
    //         inputOutputBench( clientSocket, clientServerSocket, "Socket" );
    //     gigaBytePerSecondSocket = gigaBytePerSecond;
    // }

    double gigaBytePerSecondInputOutputStream;
    {
        INIT_SERVER

        {

            hub::output::OutputStream outputStream( FILE_NAME, SERVER_PORT, "127.0.0.1", false );

            hub::input::InputStream inputStream( FILE_NAME, SERVER_PORT );

            const auto& [durationInMillisecond, gigaBytePerSecond] =
                inputOutputBench( inputStream, outputStream, "InputOutputStrem" );
            gigaBytePerSecondInputOutputStream = gigaBytePerSecond;
        }
    }

#ifdef HUB_TESTS_MQTT_FOUND
    // raw mqtt
    double gigaBytesPerSecondMqtt;
    {
        const auto& [data, size] = generateTestData();
        std::cout << "[test][Mqtt] data ready" << std::endl;

        const std::string topicName = FILE_NAME;
        const std::string ip        = "localhost:1883";
        mqtt::client inputClient( ip, "consumer", mqtt::create_options( MQTTVERSION_5 ) );
        inputClient.connect();
        inputClient.subscribe( topicName );
        inputClient.start_consuming();
        std::cout << "[test][Mqtt] input client connected" << std::endl;

        mqtt::client outputClient( ip, "producer", mqtt::create_options( MQTTVERSION_5 ) );
        outputClient.connect();
        std::cout << "[test][Mqtt] output client connected" << std::endl;
        const mqtt::message_ptr outputMsgPtr = mqtt::make_message( topicName, "" );
        //        outputMsgPtr->set_retained(true);
        outputMsgPtr->set_qos( 2 ); // no packet lost
        // outputMsgPtr->set_qos( 0 );

        mqtt::const_message_ptr inputMsgPtr;
        std::cout << "[test][Mqtt] start streaming" << std::endl;
        const auto& start2 = std::chrono::high_resolution_clock::now();

        auto thread = std::thread( [&]() {
            for ( int i = 0; i < s_nIteration; ++i ) {
                outputMsgPtr->set_payload( data, size );
                outputClient.publish( outputMsgPtr );
            }
        } );
        //        }

        for ( int i = 0; i < s_nIteration; ++i ) {
            inputMsgPtr = inputClient.consume_message();

            const auto& payload   = inputMsgPtr->get_payload();
            const auto* inputData = payload.data();

#    ifdef DEBUG
            assert( memcmp( data, inputData, size ) == 0 );
#    endif
        }
        thread.join();
        inputClient.stop_consuming();

        const auto& end2 = std::chrono::high_resolution_clock::now();
        std::cout << "[test][Mqtt] end streaming" << std::endl;
        const auto& duration2 =
            std::chrono::duration_cast<std::chrono::nanoseconds>( end2 - start2 ).count();
        gigaBytesPerSecondMqtt = ( 2 * size * s_nIteration ) / (double)duration2;

        std::cout << "[test][Mqtt] writing/reading rate: " << gigaBytesPerSecondMqtt << " Go/s"
                  << std::endl;
        std::cout << "[test][Mqtt] total time: " << duration2 / 1'000'000.0 << " ms" << std::endl;
    }
#endif

    const auto ratio = gigaBytePerSecondInputOutputStream / gigaBytesPerSecondMqtt;
    CHECK_DECLINE( ratio, "InputOutput:Stream/Mqtt", "/" );

    //    enum class Implement {
    //        SERVER = 0,
    // #ifdef HUB_BUILD_MQTT
    //        MQTT,
    // #endif
    //        COUNT,
    //    };
    //    static std::string implements2string[static_cast<int>( Implement::COUNT )] = { "Server",
    // #ifdef HUB_BUILD_MQTT
    //                                                                                   "Mqtt"
    // #endif
    //    };
    //    double megaBytesPerSecondsImpls[(int)Implement::COUNT] = { 0 };

    //    //    for (const auto & implement : implements)
    //    for ( int i = 0; i < (int)Implement::COUNT; ++i ) {
    //        const auto implement     = Implement( i );
    //        const auto implementStr  = implements2string[(int)implement];
    //        const std::string header = "[test][" + implementStr + "Impl] ";

    //        std::cout << header << "############################### server start" << std::endl;
    //        std::cout << header << "server end ------------------------------" << std::endl;

    //        {
    //            std::cout << header << "############################### outputStream start"
    //                      << std::endl;

    //            std::unique_ptr<hub::sensor::OutputSensor> outputSensor;

    //            switch ( implement ) {
    //            case Implement::SERVER:
    //                outputSensor = std::make_unique<hub::sensor::OutputSensor>(
    //                    hub::sensor::SensorSpec( "sensorName", { { { width, height },
    //                    hub::sensor::Format::BGR8 } } ), hub::output::OutputStreamServer(
    //                    FILE_NAME ) );
    //                break;
    // #ifdef HUB_BUILD_MQTT
    //            case Implement::MQTT:
    //                outputSensor = std::make_unique<hub::sensor::OutputSensor>(
    //                    hub::sensor::SensorSpec( "sensorName", { { { width, height },
    //                    hub::sensor::Format::BGR8 } } ), hub::output::OutputStreamMqtt( FILE_NAME
    //                    ) );
    //                break;
    // #endif
    //            default:
    //                assert( false );
    //                break;
    //            }

    //            //            hub::sensor::OutputSensor outputSensor(
    //            //                hub::sensor::SensorSpec( "sensorName", { { { width, height },
    //            //                hub::sensor::Format::BGR8 } } ), OutputStream( FILE_NAME )
    //            //            );

    //            std::cout
    //                << "[test][InputOutputSensor] ############################### inputStream
    //                start"
    //                << std::endl;
    //            std::unique_ptr<hub::sensor::InputSensor> inputSensor;

    //            switch ( implement ) {
    //            case Implement::SERVER:
    //                inputSensor = std::make_unique<hub::sensor::InputSensor>(
    //                    hub::input::InputStreamServer( FILE_NAME ) );
    //                break;
    // #ifdef HUB_BUILD_MQTT
    //            case Implement::MQTT:
    //                inputSensor =
    //                    std::make_unique<hub::sensor::InputSensor>( hub::input::InputStreamMqtt(
    //                    FILE_NAME ) );
    //                break;
    // #endif
    //            default:
    //                assert( false );
    //                break;
    //            }

    //            //            hub::sensor::InputSensor inputSensor(
    //            //                hub::input::InputStream( FILE_NAME ) );

    //            const auto& inputSensorSpec = inputSensor->getSpec();
    //            CHECK( inputSensorSpec.getAcquisitionSize() == dataSize );
    //            CHECK( inputSensorSpec.getSensorName() == "sensorName" );

    //            const auto& resolutions = inputSensorSpec.getResolutions();
    //            CHECK( resolutions.size() == 1 );
    //            CHECK( resolutions[0].first.size() == 2 );
    //            CHECK( resolutions[0].first.at( 0 ) == width );
    //            CHECK( resolutions[0].first.at( 1 ) == height );
    //            CHECK( resolutions[0].second == hub::sensor::Format::BGR8 );
    //            std::cout << header << "inputStream end ---------------------------------" <<
    //            std::endl;

    //            std::cout << header << "############################### send acquisitions" <<
    //            std::endl; const auto& start2 = std::chrono::high_resolution_clock::now(); auto
    //            thread        = std::thread( [&]() {
    //                for ( int i = 0; i < nAcqs; ++i ) {
    //                    *outputSensor << acqs.at( i );
    //                }
    //            } );

    //            for ( int i = 0; i < nAcqs; ++i ) {
    //                hub::sensor::Acquisition acq;
    //                *inputSensor >> acq;
    // #ifdef DEBUG
    //                //                CHECK( acq == acqs.at( i ) );
    //                assert( acq == acqs.at( i ) );
    // #endif
    //            }
    //            thread.join();
    //            const auto& end2 = std::chrono::high_resolution_clock::now();
    //            const auto& duration2 =
    //                std::chrono::duration_cast<std::chrono::milliseconds>( end2 - start2
    //                ).count();
    //            const auto& bytes2                       = dataSize * nAcqs;
    //            const auto& bytesPerSeconds2             = 1000.0 * bytes2 / duration2;
    //            megaBytesPerSecondsImpls[(int)implement] = bytesPerSeconds2 / 1000'000.0;

    //            std::cout << header << "Mega byte wrote : " << bytes2 / 1000'000.0 << " Mo"
    //                      << std::endl;
    //            std::cout << header
    //                      << "Mega byte per second : " << megaBytesPerSecondsImpls[(int)implement]
    //                      << " Mo/s" << std::endl;
    //        }

    //        // #ifdef WIN32
    //        // #    ifdef DEBUG
    //        //         checkRatio( ratio, 20 );
    //        // #    else
    //        //         checkRatio( ratio, 40 );
    //        // #    endif
    //        // #else
    //        //         if ( hostname == "msi" ) { checkRatio( ratio, 50, 15 ); }
    //        //         else {
    //        // #ifdef DEBUG
    //        //             checkRatio( ratio, 50, 10 );
    //        // #else
    //        //             checkRatio( ratio, 40, 10 );
    //        // #endif
    //        //         }
    //        // #endif
    //    }

    //    std::cout << std::endl;

    //    std::cout << "######################################## RESUME "
    //                 "########################################"
    //              << std::endl;

    //    //    std::cout << std::endl;
    //    REPORT( "[test][ClientSocket] Mega byte per second : " << megaBytesPerSecondsClientSocket
    //                                                           << " Mo/s" );
    // #ifdef HUB_TESTS_MQTT_FOUND
    //    REPORT( "[test][Mqtt] Mega byte per second : " << megaBytesPerSecondsMqtt << " Mo/s" );
    // #endif

    //    for ( int i = 0; i < (int)Implement::COUNT; ++i ) {
    //        const auto implement     = Implement( i );
    //        const auto implementStr  = implements2string[(int)implement];
    //        const std::string header = "[test][" + implementStr + "Impl] ";
    //        REPORT( header << "Mega byte per second : " <<
    //        megaBytesPerSecondsImpls[(int)implement]
    //                       << " Mo/s" );
    //    }

    //    //    std::cout << "[test][InputOutputSensor] Mega byte per second : " <<
    //    megaBytesPerSeconds2
    //    //              << " Mo/s" << std::endl;

    //    //    std::cout << std::endl;
    //    REPORT_NEW_LINE;

    //    double megaBytesPerSecondsServerImpl = megaBytesPerSecondsImpls[(int)Implement::SERVER];
    // #ifdef HUB_BUILD_MQTT
    //    double megaBytesPerSecondsMqttImpl = megaBytesPerSecondsImpls[(int)Implement::MQTT];
    // #endif

    //    double ratio;
    // #ifdef HUB_BUILD_SERVER
    //    // server performance with viewer and nativeViewer running
    //    ratio = 100.0 * megaBytesPerSecondsServerImpl / megaBytesPerSecondsClientSocket;
    //    //    std::cout << "[Hub/ClientSocket] ratio : " << ratio << " %" << std::endl;
    //    //    checkRatio( ratio, 50, 50, "Hub/ClientSocket" );
    //    if ( hostname == "asus-b450" ) // linux CI runner
    //    {
    // #    ifdef DEBUG
    //        checkRatio( ratio, 30, 10, "Hub/ClientSocket" );
    // #    else
    //        checkRatio( ratio, 25, 10, "Hub/ClientSocket" );
    // #    endif
    //    }
    //    else if ( hostname == "gigabyte-Z370P" ) { // windows CI runner
    // #    ifdef DEBUG
    //        checkRatio( ratio, 10, 10, "Hub/ClientSocket" );
    // #    else
    //        checkRatio( ratio, 10, 10, "Hub/ClientSocket" );
    // #    endif
    //    }
    //    else if ( hostname == "Mac-mini-de-gauthier.local" ) { // macOs CI runner
    // #    ifdef DEBUG
    //        checkRatio( ratio, 40, 30, "Hub/ClientSocket" );
    // #    else
    //        checkRatio( ratio, 40, 30, "Hub/ClientSocket" );
    // #    endif
    //    }
    //    else { checkRatio( ratio, 50, 50, "Hub/ClientSocket" ); }

    // #endif

    // #ifdef HUB_TESTS_MQTT_FOUND
    //     ratio = 100.0 * megaBytesPerSecondsMqtt / megaBytesPerSecondsClientSocket;
    //     //    std::cout << "[Mqtt/ClientSocket] ratio : " << ratio << " %" << std::endl;
    //     checkRatio( ratio, 12, 12, "Mqtt/ClientSocket" );
    // #endif

    // #ifdef HUB_BUILD_MQTT
    //     ratio = 100.0 * megaBytesPerSecondsMqttImpl / megaBytesPerSecondsClientSocket;
    //     //    std::cout << "[MqttStream/ClientSocket] ratio : " << ratio << " %" << std::endl;
    //     checkRatio( ratio, 4, 4, "MqttImpl/ClientSocket" );

    //    ratio = 100.0 * megaBytesPerSecondsMqttImpl / megaBytesPerSecondsMqtt;
    //    //    REPORT("[MqttImpl/Mqtt] ratio : " << ratio << " %");
    //    checkRatio( ratio, 50, 50, "MqttImpl/Mqtt" );
    // #endif

    //    std::cout << std::endl;

    // #ifdef HUB_BUILD_SERVER
    // #    ifdef HUB_TESTS_MQTT_FOUND
    //     ratio = 100.0 * megaBytesPerSecondsMqtt / megaBytesPerSecondsServerImpl;
    //     //    REPORT("[Mqtt/Hub] ratio : " << ratio << " %");
    //     checkRatio( ratio, 50, 50, "Mqtt/Hub" );

    // #    endif
    //     //    checkRatio(ratio, 5, 5);

    // #    ifdef HUB_BUILD_MQTT
    //     ratio = 100.0 * megaBytesPerSecondsMqttImpl / megaBytesPerSecondsServerImpl;
    //     //    REPORT("[MqttImpl/Hub] ratio : " << ratio << " %");
    //     //    checkRatio(ratio, 12, 5);
    //     checkRatio( ratio, 50, 50, "MqttImpl/Hub" );
    // #    endif

    //    //    std::cout << std::endl;
    //    REPORT_NEW_LINE;

    // #    ifdef HUB_TESTS_MQTT_FOUND
    //     REPORT(
    //         "Hub server is " << megaBytesPerSecondsImpls[(int)Implement::SERVER] /
    //                                 megaBytesPerSecondsMqtt
    //                          << " more efficient than raw mqtt stream (Qos = 0 -> can lost
    //                          packet)" );
    // #    endif
    // #    ifdef HUB_BUILD_MQTT
    //     REPORT( "Hub server is " << megaBytesPerSecondsImpls[(int)Implement::SERVER] /
    //                                     megaBytesPerSecondsImpls[(int)Implement::MQTT]
    //                              << " more efficient than mqtt implement (Qos = 2 -> no loss)" );
    // #    endif
    // #endif

    //    std::cout << std::endl;

    //    //    const auto ratio = 100.0 * megaBytesPerSeconds2 / megaBytesPerSeconds;
    //    //    std::cout << "[test][ClientSocket/InputOutputSensor] ratio : " << ratio << " %" <<
    //    //    std::endl;

    //    std::cout << "[test] tested on machine: '" << hostname << "'" << std::endl;

    //    delete[] datas;
    //    //    delete[] data2;
}
