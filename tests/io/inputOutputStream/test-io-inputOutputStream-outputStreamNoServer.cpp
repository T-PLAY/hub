
#include "io/test_io_common.hpp"
#include "test_common.hpp"

#include <io/output/OutputStream.hpp>

TEST_CASE( "OutputStream no server test" ) {
    TEST_BEGIN()

    const auto port = GET_RANDOM_PORT;

    {
        try {
            hub::output::OutputStream outputStream( hub::io::Header {}, FILE_NAME, port );
            CHECK( false );
        }
        catch ( hub::net::system::SocketSystem::exception& ex ) {
            CHECK( true );
            std::cout << "[test] catch exception : " << ex.what() << std::endl;
        }
    }
    std::cout << "[test] outputStream ended" << std::endl;
    TEST_END()
}
