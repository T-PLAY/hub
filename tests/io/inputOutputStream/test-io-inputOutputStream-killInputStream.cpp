// #include <catch2/catch_test_macros.hpp>
// #define HUB_DEBUG_INPUT
// #define HUB_DEBUG_OUTPUT
// #define HUB_DEBUG_SOCKET

#include "io/test_io_common.hpp"
#include "test_common.hpp"

#include <core/Utils.hpp>
#include <impl/server2/Server.hpp>
#include <impl/server2/io/input/InputStreamServer.hpp>
#include <io/input/InputStream.hpp>
#include <io/output/OutputStream.hpp>
#include <thread>

TEST_CASE( "InputOutputStream test" ) {
    const auto hostname = hub::utils::getHostname();
    const auto port     = GET_RANDOM_PORT;

    // not allowed
    {
        hub::Server server( port );
        server.asyncRun();

        {
            hub::output::OutputStream outputStream( "streamName",  port );
            { hub::input::InputStream inputStream( "streamName",  port ); }
            std::cout << "[test] inputStream ended" << std::endl;
        }

        std::cout << "[test] outputStream ended" << std::endl;
    }
    std::cout << "[test] server ended" << std::endl;

    std::cout << "[test] tested on machine: '" << hostname << "'" << std::endl;
}
