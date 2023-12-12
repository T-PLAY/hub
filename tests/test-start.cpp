#include "test_common.hpp"

#include <filesystem>

// #ifdef HUB_TEST_ALL
// #include <impl/server2/Server.hpp>
// #endif

TEST_CASE( "Test start" ) {

    const std::string randomUsedPortsFilename = "randomUsedPorts.txt";
    if ( std::filesystem::exists( randomUsedPortsFilename ) ) {
        std::filesystem::remove( randomUsedPortsFilename );
    }

    const std::string durationTestsFilename = "duration_tests.txt";
    if ( std::filesystem::exists( durationTestsFilename ) ) {
        std::filesystem::remove( durationTestsFilename );
    }

    // #ifdef HUB_TEST_ALL
    // hub::Server server;
    // server.asyncRun();
    // #endif

    START_REPORT();
}
