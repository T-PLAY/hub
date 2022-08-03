

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

#include "export.h"

#include "stream.h"

//#pragma once
//#ifdef DLLPROJECT_EXPORTS
//#   define EXPORT __declspec(dllexport)
//#else
//#   define EXPORT __declspec(dllimport)
//#endif

// This class is exported from the dll

//namespace Native {

//extern "C" SRC_API
//typedef  struct _Acq{
//    long long start;
//    long long end;
//    const unsigned char * data;
//} Acq;


// // Single line function declarations
//extern "C" SRC_API int createInputStream(const char * sensorName);
//extern "C" SRC_API int getAcquisitionSize(int id);
//extern "C" SRC_API bool getAcquisition(int id, long long * start, long long * end, unsigned char * data);

extern "C"
{
    SRC_API InputStream* createInputStream( const char* sensorName );
    SRC_API void freeInputStream( InputStream* inputStream );
    SRC_API int getAcquisitionSize( InputStream* inputStream );
    SRC_API bool getData( InputStream* inputStream, unsigned char* data );
    SRC_API bool getAcquisition( InputStream* inputStream,
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

#endif
