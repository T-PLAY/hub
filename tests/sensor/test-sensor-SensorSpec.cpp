// #define HUB_DEBUG_INPUT
// #define HUB_DEBUG_OUTPUT

#include <catch2/catch_test_macros.hpp>

//#include <sensor/InputSensor.hpp>
//#include <sensor/OutputSensor.hpp>
// #include <io/File.hpp>

#include <core/io/Archive.hpp>
#include <sensor/SensorSpec.hpp>
//#include <core/Serializer.hpp>

#include <any>

TEST_CASE( "SensorSpec test" ) {

    hub::io::Archive archive;

    hub::MetaData metaData;
    metaData["hello"] = 5;
    metaData["name"]  = "gauthier";
    hub::sensor::SensorSpec sensorSpec{"sensorName", {}, metaData};
//    sensorSpec.resolution = hub::make_matrix(hub::make_matrix<double>(), hub::make_matrix<int, 640, 480>());
//    sensorSpec.sensorName = "sensorName";
//    auto & resolution = sensorSpec.resolution;
//    resolution.push_back(make_re)
//    auto & metaData = sensorSpec.metaData;
    archive.write(sensorSpec);
    hub::sensor::SensorSpec sensorSpec_read;
    archive.read(sensorSpec_read);
    assert(sensorSpec == sensorSpec_read);
//    static_assert(! hub::Serializer::Writable_v<hub::sensor::SensorSpec>);
//    static_assert(hub::serializer::Readable_v<hub::Any>);
//    static_assert(hub::serializer::Readable_v<hub::sensor::SensorSpec::MetaData>);
    static_assert(! hub::Serializer::Serializable<hub::sensor::SensorSpec>());
    static_assert(! hub::Serializer::Serializable<hub::Any>());
    static_assert(! hub::Serializer::Serializable<hub::MetaData>());
    return;



//    hub::Any any = 5;
//    hub::Any any2 = std::move(any);
//    archive.write(any2);
//    hub::Any any_read;
//    archive.read(any_read);
//    assert(any2 == any_read);
//    return;


//    std::map<std::string, int> map;
//    map["a"] = 5;
//    map["b"] = 6;
//    archive.write(map);
//    decltype(map) map_read;
//    archive.read(map_read);
//    assert(map == map_read);
//    auto first = (*map.begin()).first;
//    auto second = (*map.begin()).second;
//    static_assert(hub::serializer::Serializable<std::map<std::string, int>>());
//    return;

//    std::pair<std::string, hub::Any> pair{"abc", 1};
//    archive.write(pair);
//    decltype(pair) pair_read;
//    archive.read(pair_read);
//    assert(pair == pair_read);
//    return;


//    hub::sensor::SensorSpec::MetaData metaData;
//    metaData["hello"] = 5;
//    metaData["name"]  = "gauthier";
//    archive.write(metaData);
//    hub::sensor::SensorSpec::MetaData metaData_read;
//    archive.read(metaData_read);
//    assert(metaData == metaData_read);
//    static_assert(hub::Serializer::Writable_v<hub::Any>);
//    static_assert(! hub::Serializer::Writable_v<hub::sensor::SensorSpec::MetaData>);
////    static_assert(hub::serializer::Readable_v<hub::Any>);
////    static_assert(hub::serializer::Readable_v<hub::sensor::SensorSpec::MetaData>);
//    static_assert(! hub::Serializer::Serializable<hub::Any>());
//    static_assert(! hub::Serializer::Serializable<hub::sensor::SensorSpec::MetaData>());
//    return;

    //    hub::sensor::Resolution::NDim nDim = { 1 };
//    hub::sensor::Resolution resolution { hub::sensor::format::BGR8, 1 };
//    hub::sensor::Resolution resolution;
//    hub::sensor::Resolutions resolutions { resolution };
//    hub::sensor::SensorSpec sensorSpec { "sensorName", resolutions, metaData };

//    CHECK( sensorSpec.getSensorName() == "sensorName" );
//    CHECK( sensorSpec.getAcquisitionSize() == 3 );

//    hub::sensor::SensorSpec sensorSpec2 { "sensorName2", { { hub::sensor::format::RGB8, 1 } } };

//    auto sensorSpecSum = sensorSpec + sensorSpec2;
//    CHECK( sensorSpecSum.getSensorName() == "sensorName + sensorName2" );
//    CHECK( sensorSpecSum.getAcquisitionSize() == 6 );

////    hub::sensor::SensorSpec sensorSpec3;
////    sensorSpec3 += sensorSpec;

////    hub::sensor::SensorSpec sensorSpec4;
////    sensorSpec4 += sensorSpec;
////    CHECK( sensorSpec3 == sensorSpec4 );

//    std::cout << sensorSpec.to_string() << std::endl;
//    std::cout << hub::sensor::SensorSpec::to_string( metaData ) << std::endl;
//    std::cout << hub::sensor::SensorSpec::to_string( metaData, true ) << std::endl;
//    std::cout << sensorSpec << std::endl;

//    hub::sensor::SensorSpec sensorSpec5( "sensorName" );
//    sensorSpec5 += hub::sensor::SensorSpec("sensorName2");

//    std::cout << sensorSpec5 << std::endl;
}
