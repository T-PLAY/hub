
#include <catch2/catch_test_macros.hpp>

#include <sensor/InputSensor.hpp>
#include <sensor/OutputSensor.hpp>
// #include <io/File.hpp>

TEST_CASE( "SensorSpec test" ) {

    hub::sensor::SensorSpec::MetaData metaData;
    metaData["hello"] = 5;
    metaData["name"]  = "gauthier";
    //    hub::sensor::Resolution::Dims dims = { 1 };
    hub::sensor::Resolution resolution { hub::sensor::format::BGR8, 1 };
    hub::sensor::Resolutions resolutions { resolution };
    hub::sensor::SensorSpec sensorSpec { "sensorName", resolutions, metaData };

    CHECK( sensorSpec.getSensorName() == "sensorName" );
    CHECK( sensorSpec.getAcquisitionSize() == 3 );

    hub::sensor::SensorSpec sensorSpec2 { "sensorName2", { { hub::sensor::format::RGB8, 1 } } };

    auto sensorSpecSum = sensorSpec + sensorSpec2;
    CHECK( sensorSpecSum.getSensorName() == "sensorName + sensorName2" );
    CHECK( sensorSpecSum.getAcquisitionSize() == 6 );

//    hub::sensor::SensorSpec sensorSpec3;
//    sensorSpec3 += sensorSpec;

//    hub::sensor::SensorSpec sensorSpec4;
//    sensorSpec4 += sensorSpec;
//    CHECK( sensorSpec3 == sensorSpec4 );

    std::cout << sensorSpec.to_string() << std::endl;
    std::cout << hub::sensor::SensorSpec::to_string( metaData ) << std::endl;
    std::cout << hub::sensor::SensorSpec::to_string( metaData, true ) << std::endl;
    std::cout << sensorSpec << std::endl;

//    hub::sensor::SensorSpec sensorSpec5( "sensorName" );
//    sensorSpec5 += hub::sensor::SensorSpec("sensorName2");

//    std::cout << sensorSpec5 << std::endl;
}
