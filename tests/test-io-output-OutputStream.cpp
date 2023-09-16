#include <catch2/catch_test_macros.hpp>

//#include "test_common.hpp"

#include <io/input/InputStream.hpp>
#include <io/output/OutputStream.hpp>
//#include <server/Server.hpp>

#ifdef HUB_BUILD_SERVER
#include <OutputSensor.hpp>
#endif

TEST_CASE( "OutputStream test" ) {


//    hub::OutputSensor outputSensor(
//        ref_sensorSpec, hub::output::OutputStream(FILE_NAME) );
#ifdef HUB_BUILD_SERVER
    hub::OutputSensor outputSensor(hub::SensorSpec{"sensorName", {{{640, 480}, hub::Format::RGB8}}}, hub::output::OutputStream(FILE_NAME));
#else
    hub::output::OutputStream outputStream(FILE_NAME);
#endif
    std::cout << "outputStream created" << std::endl;

    {
        try {
            // unable to open same stream name twice
//                                                 hub::net::ClientSocket( ipv4, port ) );
//                                                 ipv4, port );
//            hub::OutputSensor outputSensor2(
//                ref_sensorSpec, hub::output::OutputStream(FILE_NAME) );
//#ifdef HUB_BUILD_SERVER
//    hub::OutputSensor outputSensor(hub::SensorSpec{}, hub::output::OutputStream(FILE_NAME));
//#else
//#endif
            hub::output::OutputStream outputStream2(FILE_NAME);
            assert(false);
        }
        catch ( std::exception& ex ) {
            std::cout << "[test] catch exception : " << ex.what() << std::endl;
            CHECK(true);
        }
    }
}
