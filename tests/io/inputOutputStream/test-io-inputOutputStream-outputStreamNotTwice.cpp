// #include <catch2/catch_test_macros.hpp>
// #define HUB_DEBUG_INPUT
// #define HUB_DEBUG_OUTPUT
// #define HUB_DEBUG_SOCKET

#include "io/test_io_common.hpp"
#include "test_common.hpp"

#include <io/output/OutputStream.hpp>

TEST_CASE( "InputOutputStream no stream twice test" ) {
    TEST_BEGIN()

    INIT_SERVER

    {
        hub::output::OutputStream outputStream( FILE_NAME, SERVER_PORT, SERVER_IP, TEST_IO_HEADER);

        try {
            hub::output::OutputStream outputStream2( FILE_NAME, SERVER_PORT, SERVER_IP, TEST_IO_HEADER );
            assert( false );
        }
        catch ( hub::net::system::SocketSystem::exception& ex ) {
            CHECK( true );
            std::cout << "[test] catch exception : " << ex.what() << std::endl;
        }
        std::cout << "[test] outputStream ending" << std::endl;
    }
    std::cout << "[test] outputStream ended" << std::endl;
    TEST_END()
}
