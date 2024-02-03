#include "test_common.hpp"

#include <filesystem>

TEST_CASE( "Test start" ) {

#if CPP_VERSION >= 17
    const std::string randomUsedPortsFilename = "randomUsedPorts.txt";
    if ( std::filesystem::exists( randomUsedPortsFilename ) ) {
        std::filesystem::remove( randomUsedPortsFilename );
    }

    const std::string durationTestsFilename = "duration_tests.txt";
    if ( std::filesystem::exists( durationTestsFilename ) ) {
        std::filesystem::remove( durationTestsFilename );
    }
#endif

    START_REPORT();
}
