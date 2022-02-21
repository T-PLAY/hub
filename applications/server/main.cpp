#include <server.h>
#include <stream.h>

int main() {

//    auto processClient = [](Socket & socket) {
//    };

    Server server(4043);
    server.run();
}
