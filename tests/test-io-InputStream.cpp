#include <catch2/catch_test_macros.hpp>

#include "test-common.hpp"

#include <Server.hpp>
#include <io/InputStream.hpp>
#include <io/OutputStream.hpp>

TEST_CASE( "InputStream test" ) {

    const std::string ipv4 = "127.0.0.1";
    const int port         = getRandomPort();

//    try {
//    hub::io::InputStream inputStream( "streamName", "", hub::net::ClientSocket(ipv4, port) );
//    } catch (hub::net::ClientSocket::exception & ex) {
//    std::cout << "[test] catch socket exception : " << ex.what() << std::endl;
//    }

//    Server server( port );
//    server.setMaxClients( 2 );
//    server.asyncRun();

//    hub::io::OutputStream outputStream("streamName", hub::net::ClientSocket(ipv4, port));

//    hub::io::InputStream inputStream2( "streamName", "", hub::net::ClientSocket(ipv4, port) );
}
