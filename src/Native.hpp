#pragma once

#include "Macros.hpp"
#include "InputSensor.hpp"

namespace hub {

extern "C"
{
    SRC_API InputSensor* createInputSensor(const char* streamName, const char* ipv4 , int port);
    SRC_API void freeInputSensor( InputSensor* inputSensor );
    SRC_API int getAcquisitionSize( InputSensor* inputSensor );
    SRC_API bool getData( InputSensor* inputSensor, unsigned char* data, int iMeasure = 0 );
//    SRC_API bool getAcquisition( InputSensor* inputSensor,
//                                 long long* start,
//                                 long long* end,
//                                 unsigned char* data );
}

} // namespace hub
