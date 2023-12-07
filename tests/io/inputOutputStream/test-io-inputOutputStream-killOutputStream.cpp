// #include <catch2/catch_test_macros.hpp>
// #define HUB_DEBUG_INPUT
// #define HUB_DEBUG_OUTPUT
// #define HUB_DEBUG_SOCKET

#include "io/test_io_common.hpp"
#include "test_common.hpp"

#include <io/input/InputStream.hpp>
#include <io/output/OutputStream.hpp>

TEST_CASE( "InputOutputStream kill output stream test" ) {
    const auto hostname = hub::utils::getHostname();

    INIT_SERVER

    {
        auto* outputStream = new hub::output::OutputStream( FILE_NAME, SERVER_PORT, "127.0.0.1", false );
        hub::input::InputStream inputStream( FILE_NAME, SERVER_PORT );

        int a = 5;
        outputStream->write( a );
        int a_read;
        inputStream.read( a_read );
        assert( a == a_read );

        delete outputStream;
        std::cout << "[test] outputStream ended" << std::endl;

        try {
            inputStream.read( a_read );
            assert( false );
        }
        catch ( hub::net::system::SocketSystem::exception& ex ) {
            CHECK( true );
            std::cout << "[test] catch exception : " << ex.what() << std::endl;
        }
    }
    std::cout << "[test] inputStream ended" << std::endl;
}
