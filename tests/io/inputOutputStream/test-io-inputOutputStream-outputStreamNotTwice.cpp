
#include "io/test_io_common.hpp"
#include "test_common.hpp"

#include <io/output/OutputStream.hpp>

TEST_CASE( "InputOutputStream no stream twice test" ) {
    TEST_BEGIN()

    INIT_SERVER

    {
        hub::output::OutputStream outputStream( TEST_IO_HEADER, FILE_NAME, SERVER_PORT );

        try {
            hub::output::OutputStream outputStream2( TEST_IO_HEADER, FILE_NAME, SERVER_PORT );
            CHECK( false );
        }
        catch ( hub::net::system::SocketSystem::exception& ex ) {
            CHECK( true );
            std::cout << "[test] catch exception : " << ex.what() << std::endl;
        }
        std::cout << "[test] outputStream ending" << std::endl;
    }
    std::cout << "[test] outputStream ended" << std::endl;
    TEST_END()
}
