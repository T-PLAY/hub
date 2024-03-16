#define HUB_DEBUG_INPUT
#define HUB_DEBUG_OUTPUT

#include "test_common.hpp"
#include "test_io_common.hpp"

#include <io/input/InputStream.hpp>
#include <io/output/OutputStream.hpp>

TEST_CASE( "InputOutputStream short destroy both test" ) {
    TEST_BEGIN()

    const auto port = GET_RANDOM_PORT;

    {
        hub::output::OutputStream outputStream( hub::io::Header {}, port );
        hub::input::InputStream inputStream( port );
    }

    TEST_END()
}
