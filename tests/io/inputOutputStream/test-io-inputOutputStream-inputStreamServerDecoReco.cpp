
#include "io/test_io_common.hpp"
#include "test_common.hpp"

#include <io/input/InputStream.hpp>
#include <io/output/OutputStream.hpp>
#include <server/Server.hpp>

TEST_CASE( "InputStream server deco reco test" ) {
    TEST_BEGIN()

    const auto port = GET_RANDOM_PORT;

    hub::Server server( port );
    server.asyncRun();

    {
        hub::output::OutputStream outputStream( hub::io::Header{ sizeof( int ) }, FILE_NAME, port );
        assert( server.nClient() == 1 );
        assert( server.nStreamer() == 1 );
        assert( outputStream.getNStreamViewer() == 0 );

        hub::input::InputStream inputStream( FILE_NAME, port );
        assert( inputStream.getHeader().getDataSize() == sizeof( int ) );
        assert( outputStream.getNStreamViewer() == 1 );

        int a = 5;
        outputStream.write( a );
        int a_read;
        inputStream.read( a_read );
        assert( a == a_read );

        server.stop();
        assert( server.nClient() == 0 );
        assert( server.nStreamer() == 0 );
        assert( outputStream.getNStreamViewer() == 1 );

        a = 6;
        outputStream.write( a );
        inputStream.read( a_read );
        assert( a == a_read );

        server.asyncRun();
        while ( server.nStreamer() != 1 ) {
            std::cout << "[test] waiting for sreamer recon ..." << std::endl;
            std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
        }
        assert( server.nStreamer() == 1 );
        assert( outputStream.getNStreamViewer() == 1 );

        a = 7;
        outputStream.write( a );
        inputStream.read( a_read );
        assert( a == a_read );
    }

    TEST_END()
}
