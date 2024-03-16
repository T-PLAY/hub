#define HUB_DEBUG_INPUT
#define HUB_DEBUG_OUTPUT

#include "test_common.hpp"
#include "test_io_common.hpp"

#include <io/output/OutputStream.hpp>

TEST_CASE( "InputOutputStream local stream destroy after creation test" ) {
    TEST_BEGIN()

    const auto port = GET_RANDOM_PORT;

    { hub::output::OutputStream outputStream( hub::io::Header {}, port ); }

    TEST_END()
}
