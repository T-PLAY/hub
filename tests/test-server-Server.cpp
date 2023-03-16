#include <catch2/catch_test_macros.hpp>

#include "test-common.hpp"

#include <server/Server.hpp>
#include <server/StreamViewerClient.hpp>

TEST_CASE( "Server test" ) {

    const std::string ipv4 = "127.0.0.1";
    const int port         = getRandomPort();

    hub::server::Server server( port );
    server.setMaxClients( 0 );
    server.asyncRun();

//    hub::server::StreamerClient streamerClient(&server, 0, hub::net::ClientSocket());
//    server.addStreamer(&streamerClient);

}
