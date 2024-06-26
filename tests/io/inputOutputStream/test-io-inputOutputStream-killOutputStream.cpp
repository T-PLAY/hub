
#include "io/test_io_common.hpp"
#include "test_common.hpp"

#include <io/input/InputStream.hpp>
#include <io/output/OutputStream.hpp>

TEST_CASE( "InputOutputStream kill output stream test" ) {
    TEST_BEGIN()

    const auto hostname = hub::utils::getHostname();

    INIT_SERVER

    {
        const hub::io::Header header { sizeof( int ) };
        auto* outputStream = new hub::output::OutputStream( header, FILE_NAME, SERVER_PORT );
        hub::input::InputStream inputStream( FILE_NAME, SERVER_PORT );

        int a = 5;
        outputStream->write( a );
        int a_read;
        inputStream.read( a_read );
        CHECK( a == a_read );

        delete outputStream;
        std::cout << "[test] outputStream ended" << std::endl;

        try {
            inputStream.read( a_read );
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
