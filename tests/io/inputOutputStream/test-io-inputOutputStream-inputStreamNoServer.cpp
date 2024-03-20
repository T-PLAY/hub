
#include "io/test_io_common.hpp"
#include "test_common.hpp"

#include <io/input/InputStream.hpp>

TEST_CASE( "InputStream no server test" ) {
    TEST_BEGIN()

    const auto port = GET_RANDOM_PORT;

    {
        try {
            hub::input::InputStream inputStream( FILE_NAME, port );
            CHECK( false );
        }
        catch ( hub::net::system::SocketSystem::exception& ex ) {
            CHECK( true );
            std::cout << "[test] catch exception : " << ex.what() << std::endl;
        }
    }
    std::cout << "[test] inputStream ended" << std::endl;
    TEST_END()
}
