
#include "io/test_io_common.hpp"
#include "test_common.hpp"

#include <io/input/InputStream.hpp>
#include <io/output/OutputStream.hpp>

TEST_CASE( "InputStream test" ) {
    TEST_BEGIN()

    INIT_SERVER

    {
        hub::output::OutputStream outputStream( TEST_IO_HEADER, FILE_NAME, SERVER_PORT );

        hub::input::InputStream inputStream( FILE_NAME, SERVER_PORT );

        inputOutputBench( inputStream, outputStream, "inputStream/outputStream" );
    }

    TEST_END()
}
