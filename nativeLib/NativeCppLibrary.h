
/* The following ifdef block is the standard way of creating macros which make
 * exporting from a DLL simpler. All files within this DLL are compiled with
 * the NATIVECPPLIBRARY_EXPORTS symbol defined on the command line.
 * This symbol should not be defined on any project that uses this DLL.
 * This way any other project whose source files include this file see
 * NATIVECPPLIBRARY_API functions as being imported from a DLL, whereas
 * this DLL sees symbols defined with this macro as being exported.
 */

#include <iostream>

#ifdef WIN32
#    ifdef NATIVECPPLIBRARY_EXPORTS
#        define NATIVECPPLIBRARY_API __declspec( dllexport )
#    else
#        define NATIVECPPLIBRARY_API __declspec( dllimport )
#    endif

#else
#    define NATIVECPPLIBRARY_API
#endif

// This class is exported from the dll
extern "C" class NATIVECPPLIBRARY_API CNativeCppLibrary;

extern NATIVECPPLIBRARY_API int nNativeCppLibrary;
NATIVECPPLIBRARY_API int fnNativeCppLibrary( void );


// // Single line function declarations
extern "C" NATIVECPPLIBRARY_API int displayNumber();
extern "C" NATIVECPPLIBRARY_API int getRandom();
extern "C" NATIVECPPLIBRARY_API int externFun();
// // Block declarations
// extern “C” {
//     NATIVECPPLIBRARY_API int displayNumber();
//     NATIVECPPLIBRARY_API int getRandom();
//     NATIVECPPLIBRARY_API int displaySum();
// }
