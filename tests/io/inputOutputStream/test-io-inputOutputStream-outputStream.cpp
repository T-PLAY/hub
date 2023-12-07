
#include "io/test_io_common.hpp"
#include "test_common.hpp"

#include <io/output/OutputStream.hpp>

TEST_CASE( "OutputStream test" ) {

    // const auto hostname = hub::utils::getHostname();
    // const auto port     = GET_RANDOM_PORT;

    {
        INIT_SERVER

        {
            hub::output::OutputStream outputStream( FILE_NAME, SERVER_PORT, "127.0.0.1", false );
            std::cout << "outputStream created" << std::endl;

            int a = 5;
            outputStream.write( a );
        }
        std::cout << "[test] outputStream ended" << std::endl;
    }
    std::cout << "[test] server ended" << std::endl;

    // std::cout << "[test] tested on machine: '" << hostname << "'" << std::endl;
}
