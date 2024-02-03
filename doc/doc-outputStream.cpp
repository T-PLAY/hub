
#include <io/OutputStream.hpp>

/// \file

int main() {

    {
        hub::io::OutputStream outputStream( "myStream",
                                            hub::net::ClientSocket( "127.0.0.1", 4042 ) );
    }

    return 0;
}
