#pragma once

#include "Macros.hpp"
#include "Sensor.hpp"

namespace hub {

extern "C"
{
    SRC_API InputSensor* createInputSensor( const char* sensorName, const char* ipv4 );
    SRC_API void freeInputSensor( InputSensor* inputSensor );
    SRC_API int getAcquisitionSize( InputSensor* inputSensor );
    SRC_API bool getData( InputSensor* inputSensor, unsigned char* data );
    SRC_API bool getAcquisition( InputSensor* inputSensor,
                                 long long* start,
                                 long long* end,
                                 unsigned char* data );
}

} // namespace hub
