
#include "io/test_io_common.hpp"
// #include "sensor/test_sensor_common.hpp"
#include "test_common.hpp"
// #include <catch2/catch_test_macros.hpp>
#include "test_sensor_common.hpp"

// #include <sensor/InputSensor.hpp>
// #include <sensor/OutputSensor.hpp>

// #include <server/Server.hpp>
// #include <net/ServerSocket.hpp>

// #include <core/Utils.hpp>

// #ifdef HUB_TESTS_MQTT_FOUND
// #    include <mqtt/client.h>
// #endif
#include <io/input/InputStream.hpp>
#include <io/output/OutputStream.hpp>

// #include <Version.h>

TEST_CASE( "sensor : InputOutputSensorSync" ) {
    //    const auto hostname = hub::utils::getHostname();
    TEST_BEGIN()

    static_assert( !hub::Endable<hub::io::InputOutputSocket> );
    static_assert( !hub::Endable<hub::input::InputStream> );

    INIT_SERVER

    using Resolution = hub::format::Y8;
    const hub::sensor::SensorSpec sensorSpec( "sensorName", hub::make_matrix<Resolution>() );
    const auto acqs = test::sensor::generateRefAcqs( 0, 10, sensorSpec );
    hub::output::OutputStream outputStream(
        hub::io::make_header( sensorSpec ), FILE_NAME, SERVER_PORT );

    using Resolution2 = hub::format::Z16;
    const hub::sensor::SensorSpec sensorSpec2( "sensorName2", hub::make_matrix<Resolution2>() );
    const auto acqs2 = test::sensor::generateRefAcqs( 5, 10, sensorSpec2 );
    hub::output::OutputStream outputStream2(
        hub::io::make_header( sensorSpec2 ), FILE_NAME + "2", SERVER_PORT );

    hub::input::InputStream inputStream( FILE_NAME, SERVER_PORT );

    hub::input::InputStream inputStream2( FILE_NAME + "2", SERVER_PORT );


    test::sensor::checkSynchronize( outputStream,
                                    sensorSpec,
                                    acqs,
                                    outputStream2,
                                    sensorSpec2,
                                    acqs2,
                                    inputStream,
                                    inputStream2 );

    // auto acq2_read = acqs2.front().clone();
    // for ( int i = 0; i < 5; ++i ) {
    //     inputStream2.read( acq2_read );
    //     std::cout << "clearing input2: read acq: " << acq2_read << std::endl;
    // }

    using Resolution3 = hub::format::Y8;
    const hub::sensor::SensorSpec sensorSpec3( "sensorName3", hub::make_matrix<Resolution3>() );
    const auto acqs3 = test::sensor::generateRefAcqs( 10, 10, sensorSpec3 );
    hub::output::OutputStream outputStream3(
        hub::io::make_header( sensorSpec3 ), FILE_NAME + "3", SERVER_PORT );

    hub::input::InputStream inputStream3( FILE_NAME + "3", SERVER_PORT );

    test::sensor::checkSynchronize( outputStream2,
                                    sensorSpec2,
                                    acqs2,
                                    outputStream3,
                                    sensorSpec3,
                                    acqs3,
                                    inputStream2,
                                    inputStream3 );

    TEST_END()
    return;

    //    // initing datum
    //    constexpr int nAcqs       = 100;
    //    constexpr int width       = 1920;
    //    constexpr int height      = 1080;
    //    constexpr size_t dataSize = width * height * 3;

    //    unsigned char* datas = new unsigned char[nAcqs * dataSize];

    //    srand( (unsigned)time( NULL ) );
    //    std::vector<hub::sensor::Acquisition> acqs( nAcqs );
    //    for ( int i = 0; i < nAcqs; ++i ) {
    //        for ( int j = 0; j < dataSize; ++j ) {
    //            datas[i * dataSize + j] = rand() % 256;
    //        }
    //    }
    ////    memset(datas, 65, nAcqs * dataSize);

    //    for ( int iAcq = 0; iAcq < nAcqs; ++iAcq ) {
    //        const unsigned char* data = &datas[iAcq * dataSize];
    //        hub::sensor::Acquisition acq( iAcq, iAcq );
    //        acq << hub::Measure( reinterpret_cast<unsigned const char*>( data ),
    //                             dataSize,
    //                             { { width, height }, hub::sensor::Format::BGR8 } );
    //        acqs.at( iAcq ) = std::move( acq );
    //    }
    //    // datum inited

    //    // raw socket
    //    double megaBytesPerSecondsClientSocket;
    //    {
    //        const std::string ipv4 = "127.0.0.1";
    //        const int port         = GET_RANDOM_PORT;
    //        // measure direct tcp stream duration
    //        std::cout << "[test][ClientSocket] start streaming" << std::endl;
    //        hub::net::ServerSocket serverSocket( port );
    //        hub::net::ClientSocket clientSocket( ipv4, port );
    //        auto clientServerSocket = serverSocket.waitNewClient();
    //        //        constexpr int packetSize = MAX_NET_BUFFER_SIZE;
    //        //        constexpr int nPart      = dataSize / packetSize;
    //        //        std::cout << "[test][ClientSocket] nPart: " << nPart << " of " << packetSize
    //        /
    //        //        1000'000.0
    //        //                  << " Mo" << std::endl;

    //        unsigned char* dataIn = new unsigned char[dataSize];
    //        const auto& start     = std::chrono::high_resolution_clock::now();
    //        {
    //            auto thread = std::thread( [&]() {
    //                for ( int iAcq = 0; iAcq < nAcqs; ++iAcq ) {
    //                    const unsigned char* data = &datas[iAcq * dataSize];
    //                    //                int uploadSize            = 0;

    //                    //                for ( int i = 0; i < nPart - 1; ++i ) {
    //                    // #ifdef OS_MACOS
    //                    //         std::cout << "[test][ClientSocket] sending part " << i <<
    //                    std::endl;
    //                    // #endif
    //                    //                auto thread = std::thread([&]() {
    //                    clientSocket.write( data, dataSize );
    //                }
    //                //                });
    //            } );
    //            // std::cout << "[test][ClientSocket] part sended " << i << std::endl;

    //            // #ifdef OS_MACOS
    //            //         std::cout << "[test][ClientSocket] reading part " << i << std::endl;
    //            // #endif
    //            for ( int iAcq = 0; iAcq < nAcqs; ++iAcq ) {
    //                const unsigned char* data = &datas[iAcq * dataSize];
    //                clientServerSocket.read( dataIn, dataSize );
    //                // std::cout << "[test][ClientSocket] part readed " << i << std::endl;
    //                //                    thread.join();

    //                //                    uploadSize += packetSize;
    //                //                }

    //                //                clientSocket.write( data + uploadSize, dataSize - uploadSize
    //                );
    //                //                clientServerSocket.read( dataIn + uploadSize, dataSize -
    //                //                uploadSize );

    // #ifdef DEBUG
    //                 assert( memcmp( data, dataIn, dataSize ) == 0 );
    // #endif
    //                 //                CHECK( memcmp( data, dataIn, dataSize ) == 0 );
    //             }

    //            thread.join();
    //        }
    //        const auto& end = std::chrono::high_resolution_clock::now();
    //        delete[] dataIn;

    //        std::cout << "[test][ClientSocket] end streaming" << std::endl;
    //        const auto& duration =
    //            std::chrono::duration_cast<std::chrono::milliseconds>( end - start ).count();
    //        const auto& bytes               = dataSize * nAcqs;
    //        const auto& bytesPerSeconds     = 1000.0 * bytes / duration;
    //        megaBytesPerSecondsClientSocket = bytesPerSeconds / 1000'000.0;

    //        std::cout << "[test][ClientSocket] Mega byte wrote : " << bytes / 1000'000.0 << " Mo"
    //                  << std::endl;
    //        std::cout << "[test][ClientSocket] Mega byte per second : "
    //                  << megaBytesPerSecondsClientSocket << " Mo/s" << std::endl;
    //    }

    // #ifdef HUB_TESTS_MQTT_FOUND
    //     // raw mqtt
    //     double megaBytesPerSecondsMqtt;
    //     {
    //         //        const std::string topicName = "sensor";
    //         const std::string topicName = FILE_NAME;
    //         std::cout << "[test][Mqtt] start streaming" << std::endl;
    //         const std::string ip = "localhost:1883";
    //         mqtt::client inputClient( ip, "consumer", mqtt::create_options( MQTTVERSION_5 ) );
    //         inputClient.connect();
    //         inputClient.subscribe( topicName );
    //         inputClient.start_consuming();

    //        //            hub::net::ClientSocket clientSocket( ipv4, port );
    //        //            auto clientServerSocket = serverSocket.waitNewClient();
    //        mqtt::client outputClient( ip, "producer", mqtt::create_options( MQTTVERSION_5 ) );
    //        outputClient.connect();
    //        const mqtt::message_ptr outputMsgPtr = mqtt::make_message( topicName, "" );
    //        //        outputMsgPtr->set_retained(true);
    //        //                outputMsgPtr->set_qos(2);
    //        outputMsgPtr->set_qos( 0 );

    //        mqtt::const_message_ptr inputMsgPtr;
    //        const auto& start2 = std::chrono::high_resolution_clock::now();
    //        //            hub::net::ServerSocket serverSocket( port );

    //        //            const int packetSize    = 2'000'000; // 2Go network memory buffer
    //        //            const int nPart         = dataSize / packetSize;
    //        auto thread = std::thread( [&]() {
    //            for ( int iAcq = 0; iAcq < nAcqs; ++iAcq ) {
    //                //                            int uploadSize = 0;

    //                outputMsgPtr->set_payload( &datas[iAcq * dataSize], dataSize );
    //                //            outputMsgPtr->set_payload( datas, dataSize );
    //                outputClient.publish( outputMsgPtr );
    //            }
    //        } );
    //        //        }

    //        for ( int iAcq = 0; iAcq < nAcqs; ++iAcq ) {
    //            inputMsgPtr = inputClient.consume_message();
    //            //            assert( inputMsgPtr != nullptr );

    //            //            const auto& payload  = inputMsgPtr->get_payload_ref();
    //            const auto& payload = inputMsgPtr->get_payload();
    //            const auto* data3   = payload.data();
    //            //            const auto data3Size = payload.size();

    //            //                for ( int i = 0; i < nPart - 1; ++i ) {
    //            //                    clientSocket.write( data + uploadSize, packetSize );
    //            //                    clientServerSocket.read( data2 + uploadSize, packetSize );

    //            //                    uploadSize += packetSize;
    //            //                }

    //            //                clientSocket.write( data + uploadSize, dataSize - uploadSize );
    //            //                clientServerSocket.read( data2 + uploadSize, dataSize -
    //            uploadSize
    //            //                );

    //            //                assert( !memcmp( data, data2, dataSize ) );
    //            //            CHECK( dataSize == data3Size );
    // #    ifdef DEBUG
    //            assert( memcmp( &datas[iAcq * dataSize], data3, dataSize ) == 0 );
    // #    endif
    //            //            assert( !memcmp( &datas[iAcq * dataSize], data3, dataSize ) );
    //        }
    //        thread.join();
    //        inputClient.stop_consuming();
    //        const auto& end2 = std::chrono::high_resolution_clock::now();
    //        std::cout << "[test][Mqtt] end streaming" << std::endl;
    //        const auto& duration2 =
    //            std::chrono::duration_cast<std::chrono::milliseconds>( end2 - start2 ).count();
    //        const auto& bytes            = dataSize * nAcqs;
    //        const auto& bytesPerSeconds2 = 1000.0 * bytes / duration2;
    //        megaBytesPerSecondsMqtt      = bytesPerSeconds2 / 1000'000.0;

    //        std::cout << "[test][Mqtt] Mega byte wrote : " << bytes / 1000'000.0 << " Mo" <<
    //        std::endl; std::cout << "[test][Mqtt] Mega byte per second : " <<
    //        megaBytesPerSecondsMqtt << " Mo/s"
    //                  << std::endl;
    //    }
    // #endif

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
