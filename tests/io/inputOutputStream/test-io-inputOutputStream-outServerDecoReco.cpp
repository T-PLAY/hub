
#include "io/test_io_common.hpp"
#include "test_common.hpp"

#include <io/output/OutputStream.hpp>
#include <server/Server.hpp>

TEST_CASE( "OutputStream server deco reco test" ) {
    TEST_BEGIN()

    const auto port = GET_RANDOM_PORT;

    hub::Server server( port );
    server.asyncRun();
    std::cout << "[test] server started" << std::endl;
    CHECK( server.nStreamer() == 0 );

    {
        hub::output::OutputStream outputStream( hub::io::Header {}, FILE_NAME, port );
        CHECK( server.nClient() == 1 );
        CHECK( server.nStreamer() == 1 );

        std::cout << "[test] server ending" << std::endl;
        server.stop();
        std::cout << "[test] server ended" << std::endl;
        CHECK( server.nClient() == 0 );
        CHECK( server.nStreamer() == 0 );

        std::cout << "[test] server starting" << std::endl;
        server.asyncRun();
        std::cout << "[test] server started" << std::endl;

        while ( server.nStreamer() != 1 ) {
            std::cout << "[test] waiting for sreamer recon ..." << std::endl;
            std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
        }
        CHECK( server.nStreamer() == 1 );
        std::cout << "[test] outputStream ending" << std::endl;
    }
    std::cout << "[test] outputStream ended" << std::endl;

    TEST_END()
}
