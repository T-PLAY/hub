
#include "io/test_io_common.hpp"
#include "test_common.hpp"

#include <io/input/InputStream.hpp>
#include <io/output/OutputStream.hpp>

TEST_CASE( "InputOutputStream retain test" ) {
    TEST_BEGIN()

    const auto hostname = hub::utils::getHostname();

    INIT_SERVER

    {
        const hub::io::Header header { sizeof( int ) };
        hub::output::OutputStream outputStream( header, FILE_NAME, SERVER_PORT );

        {
            hub::input::InputStream inputStream( FILE_NAME, SERVER_PORT );
            assert( inputStream.isOpen() );

            int a = 5;
            outputStream.write( a );
            std::cout << "[test] write done" << std::endl;
            int a_read;
            inputStream.read( a_read );
            std::cout << "[test] read done" << std::endl;
            assert( a == a_read );

            hub::input::InputStream inputStream2( FILE_NAME, SERVER_PORT );
            assert( inputStream2.isOpen() );

            a = 6;
            outputStream.write( a );
            inputStream.read( a_read );
            assert( a == a_read );
            inputStream2.read( a_read );
            assert( a == a_read );

            a = 7;
            outputStream.setRetain( true );
            outputStream.write( a );
            outputStream.setRetain( false );

            hub::input::InputStream inputStream3( FILE_NAME, SERVER_PORT );
            assert( inputStream3.isOpen() );
            inputStream.read( a_read );
            assert( a == a_read );
            inputStream2.read( a_read );
            assert( a == a_read );
            inputStream3.read( a_read );
            assert( a == a_read );
        }
        std::cout << "[test] inputStream ended" << std::endl;
    }
    std::cout << "[test] outputStream ended" << std::endl;

    std::cout << "[test] tested on machine: '" << hostname << "'" << std::endl;
    TEST_END()
}
