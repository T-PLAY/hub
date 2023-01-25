

#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

#include <Macros.hpp>
#include <SensorSpec.hpp>

// #if ( __cplusplus >= 202001L )
// #else
// #    error "only C++20 accepted"
// #endif

/// \file

int main() {

    // clang-format off
    {
        // startConstruction
// example of rgb camera sensor
hub::SensorSpec::MetaData metaData;
metaData["iso"] = 100;
metaData["fov"] = 60.0;
hub::SensorSpec rgbCamSpec { "rgbCam", { { { 1920, 1080 }, hub::Format::RGB8 } }, metaData };
        // endConstruction
    }

    {
        // startConstruction2
// example of two-sensor 6 Degree-Of-Freedom (6DOF) motion tracking system
// with 2 sensors acquiring data at the same time (hardware synchronized timestamp)
hub::SensorSpec trackingSystemSpec { "6DOF_Tracker", { { { 2 }, hub::Format::DOF6 } } };
// or
hub::SensorSpec trackingSystemSpec2 {
    "6DOF_Tracker", { { { 1 }, hub::Format::DOF6 }, { { 1 }, hub::Format::DOF6 } } };
        // endConstruction2
    }

    {
        // startConstruction3
// example of sensor with many cameras (RGB and Infrared lenses in the same medium)
// cameras acquiring data at the same time (hardware synchronized timestamp)
hub::SensorSpec::MetaData metaData;
const double offsetRGBLens[16] = {
    1.0,  0.0,  0.0,  0.0,
    0.0,  1.0,  0.0,  0.0,
    0.0,  0.0,  1.0,  0.0,
    0.0,  10.0, 20.0, 1.0 }; // offset specification between the 2 lenses (RGB and Infrared)
metaData["offsetRGBLens"] = offsetRGBLens;
hub::SensorSpec multiCamSpec { "MultiCam (Depth, RGB, Infrared)", {
                 { { 640,  480 }, hub::Format::Z16 },
                 { { 1280, 960 }, hub::Format::RGB8 },
                 { { 640,  480 }, hub::Format::Y8 } },
                 metaData };

        // endConstruction3
    }
    // clang-format on

    hub::SensorSpec::MetaData metaData;
    hub::Format format = hub::Format::BGR8;
    hub::Dims dims     = { 1 };
    hub::Resolutions resolutions { { dims, format } };
    hub::SensorSpec sensorSpec { "sensorName", { { dims, hub::Format::BGR8 } }, metaData };
    hub::SensorSpec sensorSpec2 {
        "sensorName", { { { 1 }, hub::Format::BGR8 }, { { 1, 2, 3 }, format } }, metaData };
    hub::SensorSpec sensorSpec3 { "sensorName", {}, metaData };
    //    static_assert(sensorSpec3.getAcquisitionSize() == 0);

#if CPLUSPLUS_VERSION == 20
    using namespace std::string_literals;
    //    constexpr auto sensorName = "sensorName"s;
    constexpr std::string_view sensorName2 = "sensorName";
    //    constexpr std::string sensorName2 = "sensorName";
    //    constexpr std::vector<std::string> myStringVec{"Stroustrup"s, "Vandevoorde"s,
    //                                                      "Sutter"s, "Josuttis"s, "Wong"s };
    //    constexpr auto sensorSpec4 = hub::SensorSpec(sensorName, {}, {});
    //    constexpr auto sensorSpec4 = hub::SensorSpec("sensorName"s, {}, {});
    //    constexpr std::vector<int> vec(2, 0) = {0, 2};
    using namespace std;
    constexpr std::array<int, 2> dims2 = { 2, 4 };
    //    constexpr auto resolution = {dims2, hub::Format::BGR8};
    //    constexpr hub::Dims dims3 = const_cast<const hub::Dims>({2, 4});
    //    constexpr auto resolutions2 = hub::Resolutions{{}, hub::Format::BGR8};
    //    constexpr hub::Dims dims2 = {2, 4};
    //    constexpr hub::Resolutions resolutions2 = {{5}, hub::Format::BGR8};
    //    constexpr std::vector<int> dims3 = const_cast<std::vector<int>>({1, 2});

    constexpr hub::SensorSpec::MetaData metaData2 = {};
    constexpr auto sensorSpec4                    = hub::SensorSpec( sensorName2, {}, metaData2 );
    static_assert( sensorSpec4.getSensorName() == "sensorName" );
    static_assert( sensorSpec4.getResolutions().size() == 0 );
    static_assert( sensorSpec4.getMetaData().size() == 0 );
    static_assert( sensorSpec4.getAcquisitionSize() == 0 );

    constexpr auto sensorSpec5 = hub::SensorSpec( "sensorName" );
    static_assert( sensorSpec5.getSensorName() == "sensorName" );
    static_assert( sensorSpec5.getAcquisitionSize() == 0 );
#endif

    return 0;
}
