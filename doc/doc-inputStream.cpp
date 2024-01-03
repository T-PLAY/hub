
#include <io/input/InputStream.hpp>

/// \file

int main() {

    {
        // clang-format off
        // startConstruction
// local server : bin/app-server is running locally
hub::input::InputStream inputStream("myStream", hub::net::ClientSocket("127.0.0.1", 4042));
        // endConstruction
        // clang-format on
    }

    return 0;
}
