

#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

#include <core/Macros.hpp>
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
hub::MetaData metaData;
metaData["iso"] = 100;
metaData["fov"] = 60.0;
hub::sensor::SensorSpec rgbCamSpec { "rgbCam", { { { 1920, 1080 }, hub::format::RGB8 } }, metaData };
        // endConstruction
    }

    {
        // startConstruction2
// example of two-sensor 6 Degree-Of-Freedom (6DOF) motion tracking system
// with 2 sensors acquiring data at the same time (hardware synchronized timestamp)
hub::sensor::SensorSpec trackingSystemSpec { "6DOF_Tracker", { { { 2 }, hub::format::Dof6 } } };
// or
hub::sensor::SensorSpec trackingSystemSpec2 {
    "6DOF_Tracker", { { { 1 }, hub::format::Dof6 }, { { 1 }, hub::format::Dof6 } } };
        // endConstruction2
    }

    {
        // startConstruction3
// example of sensor with many cameras (RGB and Infrared lenses in the same medium)
// cameras acquiring data at the same time (hardware synchronized timestamp)
hub::MetaData metaData;
hub::format::Mat4 offsetRGBLens {
    1.0,  0.0,  0.0,  0.0,
    0.0,  1.0,  0.0,  0.0,
    0.0,  0.0,  1.0,  0.0,
    0.0,  10.0, 20.0, 1.0 }; // offset specification between the 2 lenses (RGB and Infrared)
metaData["offsetRGBLens"] = offsetRGBLens;
hub::sensor::SensorSpec multiCamSpec { "MultiCam (Depth, RGB, Infrared)", {
                 { { 640,  480 }, hub::format::Z16 },
                 { { 1280, 960 }, hub::format::RGB8 },
                 { { 640,  480 }, hub::format::Y8 } },
                 metaData };

        // endConstruction3
    }
    // clang-format on

    hub::MetaData metaData;
    hub::format format = hub::format::BGR8;
    hub::NDim nDim     = { 1 };
    hub::Resolutions resolutions { { nDim, format } };
    hub::sensor::SensorSpec sensorSpec { "sensorName", { { nDim, hub::format::BGR8 } }, metaData };
    hub::sensor::SensorSpec sensorSpec2 {
        "sensorName", { { { 1 }, hub::format::BGR8 }, { { 1, 2, 3 }, format } }, metaData };
    hub::sensor::SensorSpec sensorSpec3 { "sensorName", {}, metaData };

#if CPP_VERSION == 20
    using namespace std::string_literals;
    constexpr std::string_view sensorName2 = "sensorName";
    using namespace std;
    constexpr std::array<int, 2> dims2 = { 2, 4 };

    constexpr hub::MetaData metaData2 = {};
    constexpr auto sensorSpec4                    = hub::sensor::SensorSpec( sensorName2, {}, metaData2 );
    static_assert( sensorSpec4.getSensorName() == "sensorName" );
    static_assert( sensorSpec4.getResolutions().size() == 0 );
    static_assert( sensorSpec4.getMetaData().size() == 0 );
    static_assert( sensorSpec4.getAcquisitionSize() == 0 );

    constexpr auto sensorSpec5 = hub::sensor::SensorSpec( "sensorName" );
    static_assert( sensorSpec5.getSensorName() == "sensorName" );
    static_assert( sensorSpec5.getAcquisitionSize() == 0 );
#endif

    return 0;
}
