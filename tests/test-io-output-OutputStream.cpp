#include <catch2/catch_test_macros.hpp>

//#include "test_common.hpp"

#include <io/input/InputStream.hpp>
#include <io/output/OutputStream.hpp>
//#include <server/Server.hpp>
//#include <OutputSensor.hpp>

TEST_CASE( "OutputStream test" ) {


//    hub::OutputSensor outputSensor(
//        ref_sensorSpec, hub::output::OutputStream(FILE_NAME) );
    hub::output::OutputStream outputStream(FILE_NAME);
    std::cout << "outputStream created" << std::endl;

    {
        try {
            // unable to open same stream name twice
//                                                 hub::net::ClientSocket( ipv4, port ) );
//                                                 ipv4, port );
//            hub::OutputSensor outputSensor2(
//                ref_sensorSpec, hub::output::OutputStream(FILE_NAME) );
            hub::output::OutputStream outputStream2(FILE_NAME);
            assert(false);
        }
        catch ( std::exception& ex ) {
            std::cout << "[test] catch exception : " << ex.what() << std::endl;
            CHECK(true);
        }
    }
}
