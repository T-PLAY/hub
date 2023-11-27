
#include "io/test_io_common.hpp"
#include "test_common.hpp"

#include <io/input/InputStream.hpp>
#include <io/output/OutputStream.hpp>

TEST_CASE( "InputStream test" ) {
    // const auto hostname = hub::utils::getHostname();

    INIT_SERVER

    {
        hub::output::OutputStream outputStream( FILE_NAME, SERVER_PORT );

        hub::input::InputStream inputStream( FILE_NAME, SERVER_PORT );

        inputOutputBench( inputStream, outputStream, "inputStream/outputStream" );
    }

    // std::cout << "[test] tested on machine: '" << hostname << "'" << std::endl;
}
