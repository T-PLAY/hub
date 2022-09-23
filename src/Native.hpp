#pragma once

#include "Macros.hpp"
#include "InputSensor.hpp"
#include "Viewer.hpp"

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
    SRC_API Viewer*
    createViewer( const char* ipv4,
                  int port,
                  void ( *onNewStreamer )( const char* streamName, const SensorSpec* sensorSpec ),
                  void ( *onDelStreamer )( const char* streamName, const SensorSpec* sensorSpec ) );
}

} // namespace hub
