
/* The following ifdef block is the standard way of creating macros which make
 * exporting from a DLL simpler. All files within this DLL are compiled with
 * the NATIVECPPLIBRARY_EXPORTS symbol defined on the command line.
 * This symbol should not be defined on any project that uses this DLL.
 * This way any other project whose source files include this file see
 * NATIVECPPLIBRARY_API functions as being imported from a DLL, whereas
 * this DLL sees symbols defined with this macro as being exported.
 */

//#include <iostream>
#include <stream.h>

#ifdef WIN32
#    ifdef NATIVECPPLIBRARY_EXPORTS
#        define NATIVECPPLIBRARY_API __declspec( dllexport )
#    else
#        define NATIVECPPLIBRARY_API __declspec( dllimport )
#    endif

#else
#    define NATIVECPPLIBRARY_API
#endif

//#pragma once
//#ifdef DLLPROJECT_EXPORTS
//#   define EXPORT __declspec(dllexport)
//#else
//#   define EXPORT __declspec(dllimport)
//#endif

// This class is exported from the dll

//namespace Native {

//extern "C" NATIVECPPLIBRARY_API
//typedef  struct _Acq{
//    long long start;
//    long long end;
//    const unsigned char * data;
//} Acq;


// // Single line function declarations
//extern "C" NATIVECPPLIBRARY_API int createInputStream(const char * sensorName);
//extern "C" NATIVECPPLIBRARY_API int getAcquisitionSize(int id);
//extern "C" NATIVECPPLIBRARY_API bool getAcquisition(int id, long long * start, long long * end, unsigned char * data);

extern "C" NATIVECPPLIBRARY_API InputStream* createInputStream(const char * sensorName);
extern "C" NATIVECPPLIBRARY_API void freeInputStream(InputStream* inputStream);
extern "C" NATIVECPPLIBRARY_API int getAcquisitionSize( InputStream* inputStream );
//extern "C" NATIVECPPLIBRARY_API bool getAcquisition(InputStream* inputStream, );
extern "C" NATIVECPPLIBRARY_API bool getAcquisition(InputStream* inputStream, long long * start, long long * end, unsigned char * data);

// // Block declarations
// extern “C” {
//     NATIVECPPLIBRARY_API int displayNumber();
//     NATIVECPPLIBRARY_API int getRandom();
//     NATIVECPPLIBRARY_API int displaySum();
// }

//}
