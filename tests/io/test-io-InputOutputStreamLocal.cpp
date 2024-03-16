
#include "test_common.hpp"
#include "test_io_common.hpp"

#include <io/input/InputStream.hpp>
#include <io/output/OutputStream.hpp>

TEST_CASE( "InputOutputStream local test" ) {
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
        CHECK( clientSocket.isConnected() );
        auto clientServerSocket = serverSocket.waitNewClient();
        CHECK( clientServerSocket.isConnected() );

        hub::io::InputOutputSocket inputOutputSocket( std::move( clientSocket ) );
        hub::io::InputOutputSocket inputOutputSocket2( std::move( clientServerSocket ) );

        const auto& [durationInMillisecond, gigaBytePerSecond] =
            inputOutputBench( inputOutputSocket, inputOutputSocket2 );
        durationInMillisecondInputOutputSocket = durationInMillisecond;
        gigaBytePerSecondInputOutputSocket     = gigaBytePerSecond;
    }

    {

        hub::output::OutputStream outputStream( TEST_IO_HEADER, port2 );

        hub::input::InputStream inputStream( port2 );
        CHECK( inputStream.getHeader() == TEST_IO_HEADER );

        const auto& [durationInMillisecond, gigaBytePerSecond] =
            inputOutputBench( inputStream, outputStream );
        durationInMillisecondInputOutputStream = durationInMillisecond;
        gigaBytePerSecondInputOutputStream     = gigaBytePerSecond;
    }

    std::cout << "------------------------------------" << std::endl;
    std::cout << "[InputOutputSocket] writing/reading rate: "
              << std::to_string( gigaBytePerSecondInputOutputSocket ) << " Go/s" << std::endl;
    std::cout << "[InputOutputSocket] total time: " << durationInMillisecondInputOutputSocket
              << " ms" << std::endl;

    std::cout << "------------------------------------" << std::endl;
    std::cout << "[InputOutputLocalStream] writing/reading rate: "
              << std::to_string( gigaBytePerSecondInputOutputStream ) << " Go/s" << std::endl;
    std::cout << "[InputOutputLocalStream] total time: " << durationInMillisecondInputOutputStream
              << " ms" << std::endl;
    std::cout << std::endl;

    const auto ratio = gigaBytePerSecondInputOutputStream / gigaBytePerSecondInputOutputSocket;
    CHECK_DECLINE( ratio, "InputOutputLocalStream:InputOutputSocket", "/" );

    TEST_END()
}
