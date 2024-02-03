
#include <sensor/OutputSensor.hpp>

#define HUB_CPP_SOURCE
#include "native_OutputSensor.hpp"

namespace hub {
namespace native {

void freeOutputSensor( sensor::OutputSensor* outputSensor ) {
    assert( outputSensor != nullptr );
    std::cout << "[Native] freeOutputSensor( " << outputSensor << ")" << std::endl;
    delete outputSensor;
}

} // namespace native
} // namespace hub
