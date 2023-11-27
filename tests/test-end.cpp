#include "test_common.hpp"

// #include <impl/server2/Server.hpp>

#ifdef HUB_SERVER_PORT
#include <impl/server2/io/StreamServer.hpp>
#endif

TEST_CASE( "Test end" ) {

#ifdef HUB_SERVER_PORT
    hub::io::StreamServer::stopServer();
#endif

    END_REPORT();

    // waiting for the latest server logs before print report.
    std::this_thread::sleep_for( std::chrono::milliseconds( 200 ) );
    PRINT_REPORT();
}
