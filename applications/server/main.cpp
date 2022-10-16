
#include <Server.hpp>
#include <Configurations.hpp>

int main(int argc, char* argv[])
{
    int port = hub::net::s_defaultServicePort;
    if (argc == 2) {
        port = atoi(argv[1]);
    }
//    port = 4043;

    Server server(port);
    server.run();
}
