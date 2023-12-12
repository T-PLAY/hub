// #include <catch2/catch_test_macros.hpp>
// #define HUB_DEBUG_INPUT
// #define HUB_DEBUG_OUTPUT
// #define HUB_DEBUG_SOCKET

#include "test_common.hpp"
#include "test_io_common.hpp"

// #include <core/Utils.hpp>
#include <io/input/InputStream.hpp>
#include <io/output/OutputStream.hpp>
// #include <server/Server.hpp>
// #include <impl/server2/Server.hpp>
// #include <impl/server2/io/input/InputStreamServer.hpp>
// #include <InputSensor.hpp>
// #include <OutputSensor.hpp>
// #include <thread>
#include <net/ClientSocket.hpp>
#include <net/ServerSocket.hpp>

TEST_CASE( "InputOutputStream test" ) {
    TEST_BEGIN()

    // const auto hostname = hub::utils::getHostname();
    // const auto port     = GET_RANDOM_PORT;

    // double durationInMillisecondSocket;
    double gigaBytePerSecondSocket;

    // raw socket speed reference
    {
        const auto port = GET_RANDOM_PORT;

        const std::string ipv4 = "127.0.0.1";

        hub::net::ServerSocket serverSocket( port );
        hub::net::ClientSocket clientSocket( ipv4, port );
        assert( clientSocket.isConnected() );
        std::cout << "clientSocket: " << clientSocket << std::endl;
        auto clientServerSocket = serverSocket.waitNewClient();
        assert( clientServerSocket.isConnected() );
        std::cout << "clientServerSocket: " << clientServerSocket << std::endl;

        hub::io::InputOutputSocket inputOutputSocket( std::move( clientSocket ) );
        hub::io::InputOutputSocket inputOutputSocket2( std::move( clientServerSocket ) );

        const auto& [durationInMillisecond, gigaBytePerSecond] =
            inputOutputBench( inputOutputSocket, inputOutputSocket2, "InputOutputSocket" );
        // durationInMillisecondSocket = durationInMillisecond;
        gigaBytePerSecondSocket = gigaBytePerSecond;
    }

    // double durationInMillisecondInputOutputStream;
    double gigaBytePerSecondInputOutputStream;

    {
        INIT_SERVER

        {
            // hub::Server server( port );
            // server.asyncRun();

            hub::output::OutputStream outputStream(
                FILE_NAME, SERVER_PORT, SERVER_IP, TEST_IO_HEADER );

            hub::input::InputStream inputStream( FILE_NAME, SERVER_PORT );

            const auto& [durationInMillisecond, gigaBytePerSecond] =
                inputOutputBench( inputStream, outputStream, "InputOutputStrem" );
            // durationInMillisecondInputOutputStream = durationInMillisecond;
            gigaBytePerSecondInputOutputStream = gigaBytePerSecond;
        }
    }

    const auto ratio = gigaBytePerSecondInputOutputStream / gigaBytePerSecondSocket;
    CHECK_DECLINE( ratio, "InputOutput:Stream/Socket", "/" );

    // std::cout << "[test] tested on machine: '" << hostname << "'" << std::endl;
    TEST_END()
}
