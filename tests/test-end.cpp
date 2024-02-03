#include "test_common.hpp"

#ifndef HUB_NON_BUILD_IO
#    include "io/test_io_common.hpp"
#    include <io/Stream.hpp>
#endif

TEST_CASE( "Test end" ) {

#ifndef HUB_NON_BUILD_SERVER
    INIT_SERVER
    hub::io::Stream::stopServer( "127.0.0.1", SERVER_PORT );

    if ( std::filesystem::exists( "hubServerPort.txt" ) ) {
        std::filesystem::remove( "hubServerPort.txt" );
    }
#endif

    END_REPORT();

    std::this_thread::sleep_for( std::chrono::milliseconds( 200 ) );
    PRINT_REPORT();
}
