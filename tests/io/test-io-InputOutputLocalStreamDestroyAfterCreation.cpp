// #include <catch2/catch_test_macros.hpp>
#define HUB_DEBUG_INPUT
#define HUB_DEBUG_OUTPUT
// #define HUB_DEBUG_SOCKET

#include "test_common.hpp"
#include "test_io_common.hpp"

// #include <core/Utils.hpp>
#include <io/input/InputStream.hpp>
#include <io/output/OutputStream.hpp>
// #include <server/Server.hpp>
// #include <impl/server2/Server.hpp>
// #include <impl/server2/io/input/InputStreamServer.hpp>
// #include <sensor/InputSensor.hpp>
// #include <sensor/OutputSensor.hpp>
// #include <thread>
// #include <net/ClientSocket.hpp>
// #include <net/ServerSocket.hpp>

TEST_CASE( "InputOutputStream short destroy both test" ) {
    TEST_BEGIN()

    const auto port = GET_RANDOM_PORT;

    {
        hub::output::OutputStream outputStream( {}, port );
        hub::input::InputStream inputStream( port );
    }

    // std::cout << "[test] tested on machine: '" << hostname << "'" << std::endl;
    TEST_END()
}
