
#include <io/OutputStream.hpp>

/// \file

int main() {

    {
        // clang-format off
        // startConstruction
// local server : bin/app-server is running locally
hub::io::OutputStream outputStream("myStream", hub::net::ClientSocket( "127.0.0.1", 4042 ) );
        // endConstruction
        // clang-format on
    }

    return 0;
}
