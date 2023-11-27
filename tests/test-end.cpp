#include "test_common.hpp"

// #include <impl/server2/Server.hpp>

// #ifdef HUB_SERVER_PORT
// #include <impl/server2/io/StreamServer.hpp>
// #endif
#include <io/Stream.hpp>

TEST_CASE( "Test end" ) {

#if defined(HUB_BUILD_IO) && defined(HUB_BUILD_SERVER) && defined(HUB_SERVER_PORT)
    // hub::impl2::io::StreamServer::stopServer();
    hub::io::Stream::stopServer();
    // hub::io::Stream
#endif

    END_REPORT();

    // waiting for the latest server logs before print report.
    std::this_thread::sleep_for( std::chrono::milliseconds( 200 ) );
    PRINT_REPORT();
}
