
 #include <catch2/catch_test_macros.hpp>

#include <io/input/InputStream.hpp>

 TEST_CASE( "Measure test" ) {
//int main() {

    try {
        hub::input::InputStream( "killServer" );
        CHECK(false);
    }
    catch ( std::exception& ex ) {
        std::cout << "[test] catch exception : " << ex.what() << std::endl;
        CHECK(true);
    }
}
