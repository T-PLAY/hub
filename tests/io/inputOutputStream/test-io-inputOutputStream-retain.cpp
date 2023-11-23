// #include <catch2/catch_test_macros.hpp>
// #define HUB_DEBUG_INPUT
// #define HUB_DEBUG_OUTPUT
// #define HUB_DEBUG_SOCKET

#include "test_common.hpp"
#include "io/test_io_common.hpp"

#include <core/Utils.hpp>
#include <io/input/InputStream.hpp>
#include <io/output/OutputStream.hpp>
// #include <server/Server.hpp>
#include <impl/server2/Server.hpp>
#include <impl/server2/io/input/InputStreamServer.hpp>
// #include <InputSensor.hpp>
// #include <OutputSensor.hpp>
#include <thread>

TEST_CASE( "InputOutputStream test" ) {
    const auto hostname = hub::utils::getHostname();
    const auto port     = GET_RANDOM_PORT;

    {
        hub::Server server( port );
        server.asyncRun();

        {
            hub::output::OutputStream outputStream( "streamName",  port );

            {
                hub::input::InputStream inputStream( "streamName", port );
                return;

                int a = 5;
                outputStream.write( a );
                std::cout << "[test] write done" << std::endl;
                int a_read;
                inputStream.read( a_read );
                std::cout << "[test] read done" << std::endl;
                assert( a == a_read );


                hub::input::InputStream inputStream2( "streamName", port );

                a = 6;
                outputStream.write( a );
                inputStream.read( a_read );
                assert( a == a_read );
                inputStream2.read( a_read );
                assert( a == a_read );

                outputStream.setRetain( true );
                a = 7;
                outputStream.write( a );

                hub::input::InputStream inputStream3( "streamName", port );
                inputStream.read( a_read );
                assert( a == a_read );
                inputStream2.read( a_read );
                assert( a == a_read );
                inputStream3.read( a_read );
                assert( a == a_read );
            }
            std::cout << "[test] inputStream ended" << std::endl;
        }
        std::cout << "[test] outputStream ended" << std::endl;
    }
    std::cout << "[test] server ended" << std::endl;

    std::cout << "[test] tested on machine: '" << hostname << "'" << std::endl;
}
