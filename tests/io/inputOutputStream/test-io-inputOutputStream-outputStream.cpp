
#include "test_common.hpp"

#include <core/Utils.hpp>
#include <impl/server2/Server.hpp>
#include <io/output/OutputStream.hpp>

TEST_CASE( "OutputStream test" ) {

    const auto hostname = hub::utils::getHostname();
    const auto port     = GET_RANDOM_PORT;

    {
        hub::Server server( port );
        server.asyncRun();

        {
            hub::output::OutputStream outputStream( FILE_NAME, port );
            std::cout << "outputStream created" << std::endl;
        }
        std::cout << "[test] outputStream ended" << std::endl;

        {
            hub::output::OutputStream outputStream( FILE_NAME, port );
            std::cout << "outputStream created" << std::endl;

            try {
                // unable to open same stream name twice
                hub::output::OutputStream outputStream2( FILE_NAME, port );
                assert( false );
            }
            catch ( std::exception& ex ) {
                std::cout << "[test] catch exception : " << ex.what() << std::endl;
                CHECK( true );
            }
        }
        std::cout << "[test] outputStream ended" << std::endl;
    }
    std::cout << "[test] server ended" << std::endl;

    std::cout << "[test] tested on machine: '" << hostname << "'" << std::endl;
}
