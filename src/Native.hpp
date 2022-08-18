#pragma once

//#define NO_NATIVE

#ifndef NO_NATIVE

/* The following ifdef block is the standard way of creating macros which make
 * exporting from a DLL simpler. All files within this DLL are compiled with
 * the SRC_EXPORTS symbol defined on the command line.
 * This symbol should not be defined on any project that uses this DLL.
 * This way any other project whose source files include this file see
 * SRC_API functions as being imported from a DLL, whereas
 * this DLL sees symbols defined with this macro as being exported.
 */

//#include <iostream>

#    include "Macros.hpp"
#    include "Sensor.hpp"

//#pragma once
//#ifdef DLLPROJECT_EXPORTS
//#   define EXPORT __declspec(dllexport)
//#else
//#   define EXPORT __declspec(dllimport)
//#endif

// This class is exported from the dll

// namespace Native {

// extern "C" SRC_API
// typedef  struct _Acq{
//     long long start;
//     long long end;
//     const unsigned char * data;
// } Acq;

// // Single line function declarations
// extern "C" SRC_API int createhub::InputSensor(const char * sensorName);
// extern "C" SRC_API int getAcquisitionSize(int id);
// extern "C" SRC_API bool getAcquisition(int id, long long * start, long long * end, unsigned char
// * data);
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

// // Block declarations
// extern “C” {
//     SRC_API int displayNumber();
//     SRC_API int getRandom();
//     SRC_API int displaySum();
// }

//}

} // namespace hub

#endif
