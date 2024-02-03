
#include "io/test_io_common.hpp"
#include "test_common.hpp"

#include <io/input/InputStream.hpp>
#include <io/output/OutputStream.hpp>

TEST_CASE( "InputOutputStream kill all test" ) {
    TEST_BEGIN()

    const auto hostname = hub::utils::getHostname();

    INIT_SERVER

    hub::output::OutputStream outputStream( TEST_IO_HEADER, FILE_NAME, SERVER_PORT );

    hub::input::InputStream inputStream( FILE_NAME, SERVER_PORT );

    TEST_END()
}
