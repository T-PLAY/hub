#include <catch2/catch_test_macros.hpp>

#include "test-common.hpp"

#include <Server.hpp>
#include <net/ClientSocket.hpp>

TEST_CASE( "ClientSocket test" ) {


    const std::string ipv4 = "127.0.0.1";
    const int port         = getRandomPort();

//    Server server( port );
//    server.setMaxClients( 1 );
//    server.asyncRun();

//    hub::net::ClientSocket clientSocket(ipv4, port);

}
