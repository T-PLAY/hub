
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
        CHECK( inputStream.getHeader() == TEST_IO_HEADER );

        const auto& [durationInMillisecond, gigaBytePerSecond] =
            inputOutputBench( inputStream, outputStream, "InputOutputStream" );
    }

    TEST_END()
}
