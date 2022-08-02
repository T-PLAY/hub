

//#define NO_NATIVE

#ifndef NO_NATIVE

/* The following ifdef block is the standard way of creating macros which make
 * exporting from a DLL simpler. All files within this DLL are compiled with
 * the NATIVE_STREAM_EXPORTS symbol defined on the command line.
 * This symbol should not be defined on any project that uses this DLL.
 * This way any other project whose source files include this file see
 * NATIVE_STREAM_API functions as being imported from a DLL, whereas
 * this DLL sees symbols defined with this macro as being exported.
 */


//#include <iostream>
#include <stream.h>

//#undef NATIVE_STREAM_API
#define NATIVE_STREAM_EXPORTS

#ifdef WIN32
#    ifdef NATIVE_STREAM_EXPORTS
#        define NATIVE_STREAM_API __declspec( dllexport )
#    else
#        define NATIVE_STREAM_API __declspec( dllimport )
#    endif

#else
#    define NATIVE_STREAM_API
#endif

//#pragma once
//#ifdef DLLPROJECT_EXPORTS
//#   define EXPORT __declspec(dllexport)
//#else
//#   define EXPORT __declspec(dllimport)
//#endif

// This class is exported from the dll

//namespace Native {

//extern "C" NATIVE_STREAM_API
//typedef  struct _Acq{
//    long long start;
//    long long end;
//    const unsigned char * data;
//} Acq;


// // Single line function declarations
//extern "C" NATIVE_STREAM_API int createInputStream(const char * sensorName);
//extern "C" NATIVE_STREAM_API int getAcquisitionSize(int id);
//extern "C" NATIVE_STREAM_API bool getAcquisition(int id, long long * start, long long * end, unsigned char * data);

extern "C" NATIVE_STREAM_API InputStream* createInputStream(const char * sensorName);
extern "C" NATIVE_STREAM_API void freeInputStream(InputStream* inputStream);
extern "C" NATIVE_STREAM_API int getAcquisitionSize( InputStream* inputStream );
extern "C" NATIVE_STREAM_API bool getData( InputStream* inputStream, unsigned char* data );
extern "C" NATIVE_STREAM_API bool getAcquisition(InputStream* inputStream, long long * start, long long * end, unsigned char * data);

// // Block declarations
// extern “C” {
//     NATIVE_STREAM_API int displayNumber();
//     NATIVE_STREAM_API int getRandom();
//     NATIVE_STREAM_API int displaySum();
// }

//}

#endif
