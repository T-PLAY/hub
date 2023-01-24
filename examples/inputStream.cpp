
#include <IO/Stream.hpp>

/// \file

int main() {

    {
        // clang-format off
        // startConstruction
// local server : bin/app-server is running locally
hub::io::InputStream inputStream("myStream", "", hub::net::ClientSocket("127.0.0.1", 4042));
        // endConstruction
        // clang-format on
    }

    return 0;
}
