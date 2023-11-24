// #include <catch2/catch_test_macros.hpp>
// #define HUB_DEBUG_INPUT
// #define HUB_DEBUG_OUTPUT
// #define HUB_DEBUG_SOCKET

#include "io/test_io_common.hpp"
#include "test_common.hpp"

#include <io/input/InputStream.hpp>
#include <io/output/OutputStream.hpp>

TEST_CASE( "InputOutputStream test" ) {
    const auto hostname = hub::utils::getHostname();
    // const auto port     = GET_RANDOM_PORT;

    INIT_SERVER

    // not allowed
    {
        // hub::Server server( port );
        // server.asyncRun();

        {
            hub::output::OutputStream outputStream( FILE_NAME, SERVER_PORT );
            {
                hub::input::InputStream inputStream( FILE_NAME, SERVER_PORT );
                // shutdown inputStream first
            }
            std::cout << "[test] inputStream ended" << std::endl;
        }
        std::cout << "[test] outputStream ended" << std::endl;
    }
    std::cout << "[test] server ended" << std::endl;

    std::cout << "[test] tested on machine: '" << hostname << "'" << std::endl;
}
