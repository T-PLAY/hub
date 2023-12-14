
#include <sensor/OutputSensor.hpp>

#define HUB_CPP_SOURCE
#include "native_OutputSensor.hpp"

namespace hub {
namespace native {

// sensor::OutputSensor* createMat4OutputSensor( const char* sensorName, const char* ipv4, int port
// ) {
//     std::cout << "[Native] createOutputSensor( " << sensorName << ")" << std::endl;
//     sensor::OutputSensor* outputSensor = nullptr;
//     try {

//         sensor::SensorSpec sensorSpec( sensorName, { { { 1 }, sensor::Format::MAT4 } } );
//         //        outputSensor = new OutputSensor(
//         //            //            std::move( sensorSpec ), sensorName, net::ClientSocket( ipv4,
//         //            port ) ); std::move( sensorSpec ), sensorName, ipv4, port );
//         outputSensor = new sensor::OutputSensor(
//             //            std::move( sensorSpec ), sensorName, net::ClientSocket( ipv4, port ) );
//             std::move( sensorSpec ),
// //            output::OutputStream( sensorName, ipv4, port ) );
//             output::OutputStream(sensorName, ipv4, port));
//     }
//     catch ( std::exception& e ) {
//         std::cout << "[Native] createOutputSensor : catch exception : " << e.what() << std::endl;
//         return nullptr;
//     }
//     return outputSensor;
// }

// bool mat4OutputSensorSendAcq( sensor::OutputSensor* outputSensor, const float* input ) {
//     assert( input != nullptr );
//     try {
//         sensor::Acquisition acq( 1, 1 );
//         data::Mat4 mat4( input );
//         acq << std::move( mat4 );
//         *outputSensor << acq;
//         std::cout << "[Native] send acq : " << acq << std::endl;
//     }
//     catch ( std::exception& e ) {
//         std::cout << "[Native] getAcquisition : catch exception : " << e.what() << std::endl;
//         return false;
//     }
//     return true;
// }

void freeOutputSensor( sensor::OutputSensor* outputSensor ) {
    assert( outputSensor != nullptr );
    std::cout << "[Native] freeOutputSensor( " << outputSensor << ")" << std::endl;
    delete outputSensor;
}

} // namespace native
} // namespace hub
