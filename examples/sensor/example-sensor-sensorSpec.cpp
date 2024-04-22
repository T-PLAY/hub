

#include <string>

#include <sensor/SensorSpec.hpp>

/// \file
/// \brief
int main() {

    {
/// startConstruction
// example of rgb camera sensor
hub::MetaData metaData;
metaData["iso"]     = 100;
metaData["fov"]     = 60.0;
using ResolutionCam = hub::MatrixXD<hub::format::RGB8, 1920, 1080>;
hub::sensor::SensorSpec rgbCamSpec { "rgbCam", ResolutionCam(), metaData };
/// endConstruction
    }

    {
/// startConstruction2
// example of two-sensor 6 Degree-Of-Freedom (6DOF) motion tracking system
// with 2 sensors acquiring data at the same time (hardware synchronized timestamp)
using ResolutionTrack = hub::MatrixXD<hub::format::Dof6, 2>;
hub::sensor::SensorSpec trackingSystemSpec { "6DOF_Tracker", ResolutionTrack() };
// or
using ResolutionTrack2 = hub::MatrixTs<hub::format::Dof6, hub::format::Dof6>;
hub::sensor::SensorSpec trackingSystemSpec2 { "6DOF_Tracker", ResolutionTrack2() };
/// endConstruction2
    }

    {
/// startConstruction3
// example of sensor with many cameras (RGB and Infrared lenses in the same medium)
// cameras acquiring data at the same time (hardware synchronized timestamp)
hub::MetaData metaData;
hub::format::Mat4 offsetRGBLens {
    1.0,  0.0,  0.0,  0.0,
    0.0,  1.0,  0.0,  0.0,
    0.0,  0.0,  1.0,  0.0,
    0.0,  10.0, 20.0, 1.0 }; // offset specification between the 2 lenses (RGB and Infrared)
metaData["offsetRGBLens"] = offsetRGBLens;
using ResolutionDepth     = hub::MatrixXD<hub::format::Z16, 640, 480>;
using ResolutionRGB       = hub::MatrixXD<hub::format::RGB8, 1280, 960>;
using ResolutionInfrared  = hub::MatrixXD<hub::format::Y8, 640, 480>;
using ResolutionMultiCam = hub::MatrixTs<ResolutionDepth, ResolutionRGB, ResolutionInfrared>;
hub::sensor::SensorSpec multiCamSpec {
    "MultiCam (Depth, RGB, Infrared)", ResolutionMultiCam(), metaData };
/// endConstruction3
    }

    return 0;
}
