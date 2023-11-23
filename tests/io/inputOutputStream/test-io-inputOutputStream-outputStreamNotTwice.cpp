// #include <catch2/catch_test_macros.hpp>
// #define HUB_DEBUG_INPUT
// #define HUB_DEBUG_OUTPUT
// #define HUB_DEBUG_SOCKET

#include "io/test_io_common.hpp"
#include "test_common.hpp"

#include <impl/server2/Server.hpp>
#include <io/output/OutputStream.hpp>

// TEST_CASE( "InputOutputStream test" ) {
int main() {

    INIT_SERVER
    std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );

    {
        hub::output::OutputStream outputStream( FILE_NAME, SERVER_PORT );
        std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );

        //        try {
        //            hub::output::OutputStream outputStream2( FILE_NAME, SERVER_PORT );
        //            assert( false );
        //        }
        //        catch ( hub::net::system::SocketSystem::exception& ex ) {
        //            CHECK( true );
        //            std::cout << "[test] catch exception : " << ex.what() << std::endl;
        //        }
    }
    std::cout << "[test] outputStream ended" << std::endl;

    std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );
}
