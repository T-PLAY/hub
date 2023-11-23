
#include "test_common.hpp"

#include <core/Utils.hpp>
#include <impl/server2/Server.hpp>
#include <io/input/InputStream.hpp>

TEST_CASE( "InputStream test" ) {

    const auto hostname = hub::utils::getHostname();
    const auto port     = GET_RANDOM_PORT;

    {
        hub::Server server( port );
        server.asyncRun();

        try {
            // unable to open non existing stream name
            hub::input::InputStream inputStream( FILE_NAME, port );
            assert( false );
        }
        catch ( std::exception& ex ) {
            std::cout << "[test] catch exception : " << ex.what() << std::endl;
            CHECK( true );
        }
    }
    std::cout << "[test] server ended" << std::endl;

    std::cout << "[test] tested on machine: '" << hostname << "'" << std::endl;
}
