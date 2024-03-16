
#include "io/test_io_common.hpp"
#include "test_common.hpp"

#include <net/ServerSocket.hpp>

#include <io/input/InputStream.hpp>
#include <io/output/OutputStream.hpp>

#ifdef HUB_USE_MQTT
#    include <mqtt/client.h>
#endif

TEST_CASE( "Server test : InputOutputStream_vs_Mqtt" ) {
    TEST_BEGIN()

#if CPP_VERSION >= 20
    const auto& [data, size] = generateTestData();
#else
    const auto& tuple = generateTestData();
    const auto& data  = std::get<0>( tuple );
    const auto& size  = std::get<1>( tuple );
#endif

    unsigned char* data_read = new unsigned char[size];
    const auto nIteration    = 100;

    const auto port = GET_RANDOM_PORT;

    double gigaBytePerSecondInputOutputStream;
    {
        // INIT_SERVER
        hub::Server server( port );
        server.asyncRun();

        {
            hub::output::OutputStream outputStream( TEST_IO_HEADER, FILE_NAME, port );
            hub::input::InputStream inputStream( FILE_NAME, port );

            const auto& start2 = std::chrono::high_resolution_clock::now();

            auto thread = std::thread( [&]() {
                for ( int i = 0; i < nIteration; ++i ) {
                    outputStream.write( data, size );
                }
            } );

            for ( int i = 0; i < nIteration; ++i ) {
                inputStream.read( data_read, size );
#ifdef DEBUG
                CHECK( memcmp( data, data_read, size ) == 0 );
#endif
            }
            thread.join();

            const auto& end2 = std::chrono::high_resolution_clock::now();
            std::cout << "[test][InputOutputStream] end streaming" << std::endl;
            const auto& duration2 =
                std::chrono::duration_cast<std::chrono::nanoseconds>( end2 - start2 ).count();
            gigaBytePerSecondInputOutputStream = ( 2 * size * nIteration ) / (double)duration2;

            std::cout << "[test][InputOutputStream] writing/reading rate: "
                      << gigaBytePerSecondInputOutputStream << " Go/s" << std::endl;
            std::cout << "[test][InputOutputStream] total time: " << duration2 / 1'000'000.0
                      << " ms" << std::endl;
        }
    }

#ifdef HUB_TESTS_MQTT_FOUND
    double gigaBytesPerSecondMqtt;
    {
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
        outputMsgPtr->set_qos( 2 ); // no packet lost

        mqtt::const_message_ptr inputMsgPtr;
        std::cout << "[test][Mqtt] start streaming" << std::endl;
        const auto& start2 = std::chrono::high_resolution_clock::now();

        auto thread = std::thread( [&]() {
            for ( int i = 0; i < nIteration; ++i ) {
                outputMsgPtr->set_payload( data, size );
                outputClient.publish( outputMsgPtr );
            }
        } );

        for ( int i = 0; i < nIteration; ++i ) {
            inputMsgPtr = inputClient.consume_message();

            const auto& payload   = inputMsgPtr->get_payload();
            const auto* inputData = payload.data();

#    ifdef DEBUG
            CHECK( memcmp( data, inputData, size ) == 0 );
#    endif
        }
        thread.join();
        inputClient.stop_consuming();

        const auto& end2 = std::chrono::high_resolution_clock::now();
        std::cout << "[test][Mqtt] end streaming" << std::endl;
        const auto& duration2 =
            std::chrono::duration_cast<std::chrono::nanoseconds>( end2 - start2 ).count();
        gigaBytesPerSecondMqtt = ( 2 * size * nIteration ) / (double)duration2;

        std::cout << "[test][Mqtt] writing/reading rate: " << gigaBytesPerSecondMqtt << " Go/s"
                  << std::endl;
        std::cout << "[test][Mqtt] total time: " << duration2 / 1'000'000.0 << " ms" << std::endl;
    }
#endif

    delete[] data_read;

    const auto ratio = gigaBytePerSecondInputOutputStream / gigaBytesPerSecondMqtt;
    CHECK_DECLINE( ratio, "InputOutput:Stream/Mqtt", "/" );

    TEST_END()
}
