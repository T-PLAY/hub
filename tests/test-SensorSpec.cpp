
#include <catch2/catch_test_macros.hpp>

#include <InputSensor.hpp>
#include <OutputSensor.hpp>
#include <io/File.hpp>
//#include <io/Ram.hpp>
//#include <Streamer.hpp>

TEST_CASE( "SensorSpec test" ) {

    hub::SensorSpec::MetaData metaData;
    metaData["hello"] = 5;
    metaData["name"]  = "gauthier";
    hub::Dims dims    = { 1 };
    hub::SensorSpec sensorSpec { "sensorName", { { dims, hub::Format::BGR8 } }, metaData };

    CHECK( sensorSpec.getSensorName() == "sensorName" );
    CHECK( sensorSpec.getAcquisitionSize() == 3 );

    hub::SensorSpec sensorSpec2 { "sensorName2", { { { 2 }, hub::Format::RGB8 } } };

    auto sensorSpecSum = sensorSpec + sensorSpec2;
    CHECK( sensorSpecSum.getSensorName() == "sensorName + sensorName2" );
    CHECK( sensorSpecSum.getAcquisitionSize() == 9 );

    hub::SensorSpec sensorSpec3;
    sensorSpec3 += sensorSpec;

    hub::SensorSpec sensorSpec4;
    sensorSpec4 += sensorSpec;
    CHECK( sensorSpec3 == sensorSpec4 );

    std::cout << sensorSpec.to_string() << std::endl;
    std::cout << hub::SensorSpec::to_string( metaData ) << std::endl;
    std::cout << hub::SensorSpec::to_string( metaData, true ) << std::endl;
    std::cout << sensorSpec << std::endl;

    hub::SensorSpec sensorSpec5( "sensorName" );
    sensorSpec5 += hub::SensorSpec();

    std::cout << sensorSpec5 << std::endl;
}
