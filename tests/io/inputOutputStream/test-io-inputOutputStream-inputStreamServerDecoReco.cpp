// #include <catch2/catch_test_macros.hpp>
// #define HUB_DEBUG_INPUT
// #define HUB_DEBUG_OUTPUT
// #define HUB_DEBUG_SOCKET

#include "io/test_io_common.hpp"
#include "test_common.hpp"

#include <io/input/InputStream.hpp>
#include <io/output/OutputStream.hpp>
#include <server/Server.hpp>

TEST_CASE( "InputStream server deco reco test" ) {
    TEST_BEGIN()

    // const auto hostname = hub::utils::getHostname();
    const auto port = GET_RANDOM_PORT;

    hub::Server server( port );
    server.asyncRun();

    {
        // hub::output::OutputStream outputStream( FILE_NAME, port );
        // const hub::io::Header header{sizeof(int)};
        hub::output::OutputStream outputStream( {sizeof(int)}, FILE_NAME, port);
        assert(server.nClient() == 1);
        assert( server.nStreamer() == 1 );
        assert(outputStream.getNStreamViewer() == 0);


        hub::input::InputStream inputStream( FILE_NAME, port );
        assert(inputStream.getHeader().getDataSize() == sizeof(int));
        assert(outputStream.getNStreamViewer() == 1);

        int a = 5;
        outputStream.write( a );
        int a_read;
        inputStream.read( a_read );
        assert( a == a_read );

        server.stop();
        assert(server.nClient() == 0);
        assert( server.nStreamer() == 0 );
        assert(outputStream.getNStreamViewer() == 1);

        a = 6;
        outputStream.write( a );
        inputStream.read( a_read );
        assert( a == a_read );

        server.asyncRun();
        // std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );
        while ( server.nStreamer() != 1 ) {
            std::cout << "[test] waiting for sreamer recon ..." << std::endl;
            std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
        }
        assert( server.nStreamer() == 1 );
        assert(outputStream.getNStreamViewer() == 1);

        a = 7;
        outputStream.write( a );
        inputStream.read( a_read );
        assert( a == a_read );
    }

    // std::cout << "[test] tested on machine: '" << hostname << "'" << std::endl;
    TEST_END()
}
