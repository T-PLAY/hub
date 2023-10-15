#include "test_common.hpp"

 TEST_CASE( "Test end" ) {

    END_REPORT();

    // waiting for the latest server killed logs before print report.
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    PRINT_REPORT();
}
