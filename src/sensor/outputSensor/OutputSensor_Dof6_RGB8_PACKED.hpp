
#pragma once

#include "OutputSensor_Instance.hpp"

namespace hub {
namespace sensor {
namespace outputSensor {

// class OutputSensor_Dof6_RGB8_Packed : protected OutputSensor_Instance
// {
//   public:
//     using OutputSensor_Instance::asyncRun;
//     using OutputSensor_Instance::run;
//     using OutputSensor_Instance::stop;

//     void routine() override {

//         constexpr int nAcq = 10;

//         hub::MetaData metaData;
//         metaData["nAcq"]     = nAcq;
//         constexpr int width  = 512;
//         constexpr int height = 512;
//         using ResolutionCam  = hub::MatrixXD<hub::format::RGB8, width, height>;
//         using Resolution     = hub::MatrixTs<hub::format::Dof6, ResolutionCam>;
//         const hub::sensor::SensorSpec sensorSpec( FILE_NAME, Resolution(), metaData );

//         hub::sensor::OutputSensorT<Resolution> outputSensor( sensorSpec, FILE_NAME );
//         auto & output = outputSensor.getOutput();
//         output.setRetain(true);
//         auto acq                 = outputSensor.acqMsg();
//         auto& start              = acq.start();
//         auto& end                = acq.end();
//         auto* imgData            = acq.get<hub::format::RGB8*>();
//         constexpr size_t imgSize = width * height;
//         auto& dof6               = acq.get<hub::format::Dof6&>();
//         dof6                     = hub::format::Dof6();

//         constexpr auto maxFps = 40.0;

//         const auto third = width / 3;

//         int dec = 0;
//         for ( int iAcq = 0; iAcq < nAcq; ++iAcq ) {
//             const auto startClock = std::chrono::high_resolution_clock::now();
//             // const auto startClock = iAcq;

//             // start  = hub::sensor::getClock();
//             start  = iAcq;
//             dof6.y = iAcq * 10;
//             assert( dof6.w0 == 1 );
//             assert( dof6.w1 == 0 );
//             assert( dof6.w2 == 0 );
//             assert( dof6.w3 == 0 );
//             for ( int i = 0; i < height; ++i ) {
//                 for ( int j = 0; j < width; ++j ) {
//                     const auto idx = i * width + j;
//                     assert( idx < imgSize );
//                     imgData[idx].r = ( i + j + dec ) % 128;
//                     imgData[idx].g = ( i + j + dec ) % 128;
//                     imgData[idx].b = ( i + j + dec ) % 128;
//                     if ( j < third ) { imgData[idx].r += 128; }
//                     else if ( j < 2 * third ) {
//                         imgData[idx].r += 128;
//                         imgData[idx].g += 128;
//                         imgData[idx].b += 128;
//                     }
//                     else { imgData[idx].b += 128; }
//                 }
//             }
//             // end = hub::sensor::getClock();
//             end = iAcq;
//             ++dec;

//             // std::cout << "acq: " << acq << std::endl;
//             outputSensor << acq;

//             const auto endClock =
//                 startClock + std::chrono::microseconds( (int)( 1'000'000 / maxFps ) );
//             std::this_thread::sleep_until( endClock );
//         }
//         output.setRetain(false);

//         while (1) {
//             std::this_thread::sleep_for(std::chrono::milliseconds(1000));
//             outputSensor << acq;
//         }
//     }

//   private:
// };

} // namespace outputSensor
} // namespace sensor
} // namespace hub
