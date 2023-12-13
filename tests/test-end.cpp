#include "test_common.hpp"
#include "io/test_io_common.hpp"

// #include <impl/server2/Server.hpp>

// #ifdef HUB_SERVICE_PORT
// #include <impl/server2/io/StreamServer.hpp>
// #endif
#include <io/Stream.hpp>

TEST_CASE( "Test end" ) {

// #if defined(HUB_BUILD_SERVER) && defined(HUB_SERVICE_PORT)
    // hub::io::Stream::stopServer();
// #endif
#if defined(HUB_BUILD_SERVER)
    INIT_SERVER
    hub::io::Stream::stopServer("127.0.0.1", SERVER_PORT);

    if (std::filesystem::exists("hubServerPort.txt")) {
        std::filesystem::remove("hubServerPort.txt");
    }
#endif

    END_REPORT();

    // waiting for the latest server logs before print report.
    std::this_thread::sleep_for( std::chrono::milliseconds( 200 ) );
    PRINT_REPORT();
}
