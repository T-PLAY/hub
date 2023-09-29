#include "test_common.hpp"

 #include <catch2/catch_test_macros.hpp>

#include <io/input/InputStream.hpp>

 TEST_CASE( "Test end" ) {

    try {
        hub::input::InputStream( "killServer" );
        CHECK(false);
    }
    catch ( std::exception& ex ) {
        std::cout << "[test] catch exception : " << ex.what() << std::endl;
        CHECK(true);
    }

    END_REPORT();

    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    PRINT_REPORT();
}
