#include <catch2/catch_test_macros.hpp>

#include "test-common.hpp"

#include <net/ServerSocket.hpp>
#include <server/Server.hpp>

TEST_CASE( "ServerSocket test" ) {

    //    hub::server::Server server()
//    const std::string ipv4 = "127.0.0.1";
    const int port         = getRandomPort() + 3;
    hub::server::Server server( port );
        server.setMaxClients( 0 );
        server.asyncRun();

    //    const std::string ipv4 = "127.0.0.1";
    //    const int port         = getRandomPort();
    try {
        hub::server::Server server2( port );
//        server2.setMaxClients(0);
        CHECK(false);
    }
    catch ( hub::net::Socket::exception & ex ) {
        std::cout << "[test] catch exception : " << ex.what() << std::endl;
        CHECK(true);
//        throw ex;
    }
    catch ( std::exception & ex ) {
        std::cout << "[test] catch exception : " << ex.what() << std::endl;
//        throw ex;
    }

    hub::net::ServerSocket serverSocket;
//    hub::net::ServerSocket serverSocket2(-1);
}
