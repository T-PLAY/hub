// #define HUB_DEBUG_INPUT
// #define HUB_DEBUG_OUTPUT
// #define HUB_DEBUG_SOCKET

#include "io/test_io_common.hpp"
#include "test_common.hpp"

#include <io/input/InputStream.hpp>

TEST_CASE( "InputOutputStream test" ) {
    const auto port = GET_RANDOM_PORT;

    {
        try {
            hub::input::InputStream inputStream( FILE_NAME, port );
            assert( false );
        }
        catch ( hub::net::system::SocketSystem::exception& ex ) {
            CHECK( true );
            std::cout << "[test] catch exception : " << ex.what() << std::endl;
        }
    }
    std::cout << "[test] inputStream ended" << std::endl;
}
