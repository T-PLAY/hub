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
// #include <net/ClientSocket.hpp>
// #include <net/ServerSocket.hpp>

// struct UserDefined {
//     int a;
//     bool b;
//     double c;
//     hub::Size_t dataSize() const { return sizeof( int ); }
//     auto toString() const {
//         return std::to_string( a ) + ":" + std::to_string( b ) + ":" + std::to_string( c );
//     }
// };

TEST_CASE( "InputOutputStream test" ) {
    TEST_BEGIN()

    const std::string ipv4 = "127.0.0.1";
    const auto port        = GET_RANDOM_PORT;
    const auto port2       = port + 1;

    double durationInMillisecondInputOutputSocket;
    double gigaBytePerSecondInputOutputSocket;

    double durationInMillisecondInputOutputStream;
    double gigaBytePerSecondInputOutputStream;

    {
        hub::net::ServerSocket serverSocket( port );
        hub::net::ClientSocket clientSocket( ipv4, port );
        assert( clientSocket.isConnected() );
        auto clientServerSocket = serverSocket.waitNewClient();
        assert( clientServerSocket.isConnected() );

        hub::io::InputOutputSocket inputOutputSocket( std::move( clientSocket ) );
        hub::io::InputOutputSocket inputOutputSocket2( std::move( clientServerSocket ) );

        const auto& [durationInMillisecond, gigaBytePerSecond] =
            inputOutputBench( inputOutputSocket, inputOutputSocket2 );
        durationInMillisecondInputOutputSocket = durationInMillisecond;
        gigaBytePerSecondInputOutputSocket     = gigaBytePerSecond;
    }

    {
        // const UserDefined userDefined { 1, true, 2.0 };
        // const auto header_ref = hub::io::make_header( userDefined );

        hub::output::OutputStream outputStream( TEST_IO_HEADER, port2 );

        hub::input::InputStream inputStream( port2 );
        // assert( inputStream.getHeader() == hub::io::Header() );
        assert( inputStream.getHeader() == TEST_IO_HEADER );

        const auto& [durationInMillisecond, gigaBytePerSecond] =
            inputOutputBench( inputStream, outputStream );
        durationInMillisecondInputOutputStream = durationInMillisecond;
        gigaBytePerSecondInputOutputStream     = gigaBytePerSecond;
    }

    std::cout << "------------------------------------" << std::endl;
    std::cout << "[InputOutputSocket] writing/reading rate: "
              << std::to_string( gigaBytePerSecondInputOutputSocket ) << " Go/s" << std::endl;
    std::cout << "[InputOutputSocket] total time: " << durationInMillisecondInputOutputSocket << " ms"
              << std::endl;

    std::cout << "------------------------------------" << std::endl;
    std::cout << "[InputOutputLocalStream] writing/reading rate: "
              << std::to_string( gigaBytePerSecondInputOutputStream ) << " Go/s" << std::endl;
    std::cout << "[InputOutputLocalStream] total time: " << durationInMillisecondInputOutputStream
              << " ms" << std::endl;
    std::cout << std::endl;

    const auto ratio = gigaBytePerSecondInputOutputStream / gigaBytePerSecondInputOutputSocket;
    CHECK_DECLINE( ratio, "InputOutputLocalStream:InputOutputSocket", "/" );

    // std::cout << "[test] tested on machine: '" << hostname << "'" << std::endl;
    TEST_END()
}
