// #define HUB_DEBUG_INPUT
// #define HUB_DEBUG_OUTPUT
// #define HUB_DEBUG_SOCKET

#include "io/test_io_common.hpp"
#include "test_common.hpp"

#include <io/input/InputStream.hpp>
#include <io/output/OutputStream.hpp>

TEST_CASE( "InputOutputStream test" ) {
    TEST_BEGIN()

    INIT_SERVER

    {

        hub::output::OutputStream outputStream( TEST_IO_HEADER, FILE_NAME, SERVER_PORT );

        hub::input::InputStream inputStream( FILE_NAME, SERVER_PORT );
        assert( inputStream.getHeader() == TEST_IO_HEADER );

        const auto& [durationInMillisecond, gigaBytePerSecond] =
            inputOutputBench( inputStream, outputStream, "InputOutputStream" );
    }

    // std::cout << "[test] tested on machine: '" << hostname << "'" << std::endl;
    TEST_END()
}
