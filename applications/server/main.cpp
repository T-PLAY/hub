
#include <Configurations.hpp>
#include <Server.hpp>

int main( int argc, char* argv[] ) {
    int port = hub::net::s_defaultServicePort;
    if ( argc == 2 ) { port = atoi( argv[1] ); }

    Server server( port );
    server.run();
}
