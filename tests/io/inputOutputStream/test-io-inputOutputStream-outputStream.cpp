
#include "io/test_io_common.hpp"
#include "test_common.hpp"

#include <io/output/OutputStream.hpp>

TEST_CASE( "OutputStream test" ) {
    TEST_BEGIN()

    {
        INIT_SERVER

        {
            const hub::io::Header header { sizeof( int ) };
            hub::output::OutputStream outputStream( header, FILE_NAME, SERVER_PORT );
            std::cout << "outputStream created" << std::endl;

            int a = 5;
            outputStream.write( a );
        }
        std::cout << "[test] outputStream ended" << std::endl;
    }
    std::cout << "[test] server ended" << std::endl;

    TEST_END()
}
