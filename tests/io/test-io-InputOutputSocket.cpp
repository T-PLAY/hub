
#include "test_common.hpp"
#include "test_io_common.hpp"

#include <io/InputOutputSocket.hpp>
#include <net/ServerSocket.hpp>

TEST_CASE( "InputOutputSocket test" ) {
    TEST_BEGIN()

    const auto hostname = hub::utils::getHostname();
    const auto port     = GET_RANDOM_PORT;

    const std::string ipv4 = "127.0.0.1";

    hub::net::ServerSocket serverSocket( port );
    hub::net::ClientSocket clientSocket( ipv4, port );

    assert( clientSocket.isConnected() );
    std::cout << "clientSocket: " << clientSocket << std::endl;
    auto clientServerSocket = serverSocket.waitNewClient();
    assert( clientServerSocket.isConnected() );
    std::cout << "clientServerSocket: " << clientServerSocket << std::endl;

    const auto& [durationInMillisecondClientSocket, gigaBytePerSecondClientSocket] =
        inputOutputBench( clientSocket, clientServerSocket, "ClientSocket" );

    hub::io::InputOutputSocket inputOutputSocket( std::move( clientSocket ) );
    hub::io::InputOutputSocket inputOutputSocket2( std::move( clientServerSocket ) );

    const std::string str = "hello";
    inputOutputSocket.write( str );
    std::string str_read;
    inputOutputSocket2.read( str_read );
    assert( str == str_read );

    const auto& [durationInMillisecondInputOutputSocket, gigaBytePerSecondInputOutputSocket] =
        inputOutputBench( inputOutputSocket, inputOutputSocket2, "InputOutputSocket" );

    const auto ratio = gigaBytePerSecondInputOutputSocket / gigaBytePerSecondClientSocket;
    CHECK_DECLINE( ratio, "InputOutputSocket/ClientSocket", "/" );

    std::cout << "[test] tested on machine: '" << hostname << "'" << std::endl;

    TEST_END()
}
