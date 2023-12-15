#define HUB_DEBUG_INPUT
#define HUB_DEBUG_OUTPUT

#include "test_common.hpp"
#include "test_io_common.hpp"

#include <io/output/OutputStream.hpp>

TEST_CASE( "InputOutputStream test" ) {
    TEST_BEGIN()

    const auto port = GET_RANDOM_PORT;

    { hub::output::OutputStream outputStream( {}, port ); }

    // std::cout << "[test] tested on machine: '" << hostname << "'" << std::endl;
    TEST_END()
}
