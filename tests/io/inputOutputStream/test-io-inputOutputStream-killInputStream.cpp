
#include "io/test_io_common.hpp"
#include "test_common.hpp"

#include <io/input/InputStream.hpp>
#include <io/output/OutputStream.hpp>

TEST_CASE( "InputOutputStream kill input stream test" ) {
    TEST_BEGIN()

    const auto hostname = hub::utils::getHostname();

    INIT_SERVER

    {

        {
            const hub::io::Header header { sizeof( int ) };
            hub::output::OutputStream outputStream( header, FILE_NAME, SERVER_PORT );
            int a = 5;
            {
                hub::input::InputStream inputStream( FILE_NAME, SERVER_PORT );
                outputStream.write( a );
                std::cout << "[test] writing done" << std::endl;
                int a_read;
                inputStream.read( a_read );
                std::cout << "[test] read done" << std::endl;
                CHECK( a == a_read );
            }
            std::cout << "[test] inputStream ended" << std::endl;

            a = 6;
            outputStream.write( a );
        }
        std::cout << "[test] outputStream ended" << std::endl;
    }
    std::cout << "[test] server ended" << std::endl;

    std::cout << "[test] tested on machine: '" << hostname << "'" << std::endl;
    TEST_END()
}
