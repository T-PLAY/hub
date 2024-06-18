

#include "test_common.hpp"

#include <algorithm>

#include <core/Utils.hpp>
#include <net/ServerSocket.hpp>

TEST_CASE( "Net test : ClientSocket" ) {
    TEST_BEGIN()

    const std::string ipv4 = "127.0.0.1";
    const auto port        = GET_RANDOM_PORT;

    std::vector<hub::Data_t> vector( 10 );
    std::generate( vector.begin(), vector.end(), std::rand );

    std::vector<hub::Data_t> vector_read( 10 );

#ifdef HUB_DEBUG_SOCKET
    std::cout << "[test] server round 1 ---------------------------------------------------"
              << std::endl;
#endif
    // ServerSocket
    {

        hub::net::ServerSocket serverSocket( port );

        // ClientSocket
        {
            hub::net::ClientSocket clientSocket( ipv4, port );
            CHECK( clientSocket.isConnected() );

            auto clientServerSocket = serverSocket.waitNewClient();
            CHECK( clientServerSocket.isConnected() );

            clientSocket.write( vector.data(), vector.size() );
            clientServerSocket.read( vector_read.data(), vector_read.size() );
            CHECK( vector == vector_read );

            clientServerSocket.write( vector.data(), vector.size() );
            clientSocket.read( vector_read.data(), vector_read.size() );
            CHECK( vector == vector_read );

            /////////////////

            hub::net::ClientSocket clientSocket2( ipv4, port );
            CHECK( clientSocket2.isConnected() );

            auto clientServerSocket2 = serverSocket.waitNewClient();
            CHECK( clientServerSocket2.isConnected() );

            clientSocket2.write( vector.data(), vector.size() );
            clientServerSocket2.read( vector_read.data(), vector_read.size() );
            CHECK( vector == vector_read );

            clientServerSocket2.write( vector.data(), vector.size() );
            clientSocket2.read( vector_read.data(), vector_read.size() );
            CHECK( vector == vector_read );
        }
#ifdef HUB_DEBUG_SOCKET
        std::cout << "[test] end clientSocket -----------------------" << std::endl;
        std::cout << std::endl;
#endif
    }
#ifdef HUB_DEBUG_SOCKET
    std::cout << "[test] end serverSocket -----------------------" << std::endl;
    std::cout << std::endl;
#endif

    //////////////////////////////////////

#ifdef HUB_DEBUG_SOCKET
    std::cout << "[test] server round 2 ---------------------------------------------------"
              << std::endl;
#endif
    // ServerSocket
    {

        hub::net::ServerSocket serverSocket( port );

        // ClientSocket
        {
            hub::net::ClientSocket clientSocket( ipv4, port );
            CHECK( clientSocket.isConnected() );

            auto clientServerSocket = serverSocket.waitNewClient();
            CHECK( clientServerSocket.isConnected() );

            clientSocket.write( vector.data(), vector.size() );
            clientServerSocket.read( vector_read.data(), vector_read.size() );
            CHECK( vector == vector_read );

            clientServerSocket.write( vector.data(), vector.size() );
            clientSocket.read( vector_read.data(), vector_read.size() );
            CHECK( vector == vector_read );
        }
#ifdef HUB_DEBUG_SOCKET
        std::cout << "[test] end clientSocket -----------------------" << std::endl;
        std::cout << std::endl;
#endif
    }
#ifdef HUB_DEBUG_SOCKET
    std::cout << "[test] end serverSocket -----------------------" << std::endl;
    std::cout << std::endl;
#endif

    TEST_END()
}
