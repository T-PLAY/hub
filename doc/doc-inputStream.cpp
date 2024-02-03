
#include <io/input/InputStream.hpp>

/// \file

int main() {

    {
        hub::input::InputStream inputStream( "myStream",
                                             hub::net::ClientSocket( "127.0.0.1", 4042 ) );
    }

    return 0;
}
