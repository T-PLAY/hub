#pragma once

//#include <fstream>
// static std::ofstream s_logFile;

//namespace hub {

#define DEBUG

// if ( !s_logFile.is_open() ){ \
// 				s_logFile.open( "log.txt", std::ios_base::beg ); \
// 			} \
// 			assert( s_logFile.is_open() ); \
//             s_logFile << str << std::endl; \

#ifdef DEBUG
#    define DEBUG_MSG( str )               \
        do {                               \
            std::cout << str << std::endl; \
        } while ( false )
#else
#    define DEBUG_MSG( str ) \
        do {                 \
        } while ( false )
#endif



//#undef SRC_API
//#ifdef WIN32
#if SRC_EXPORTS
#   define SRC_API __declspec( dllexport )
#elif defined SRC_STATIC
#   define SRC_API
#else
#   define SRC_API __declspec( dllimport )
#endif

//#undef SRC_API
//#define SRC_API

//#ifdef WIN32
//
//#ifndef _WINDOWS_
//#define WIN32_LEAN_AND_MEAN
//#define NOMINMAX
//#include <windows.h>
//#undef WIN32_LEAN_AND_MEAN
//#endif
//
//#include <WS2tcpip.h>
//#include <winsock2.h>
//
//#pragma comment( lib, "ws2_32.lib" )
//
////#ifndef NOMINMAX
////#define NOMINMAX
////#endif
////#include <windows.h>
//
//#endif

#if WIN32
//	#pragma warning(disable: 4244) // Conversion from double to float loses data.
    #pragma warning(disable: 4251) // stl dllexports
//	#pragma warning(disable: 4267) // conversion from size_t to uint
//	#pragma warning(disable: 4275) // non - DLL-interface class 'class_1' used as base for DLL-interface class 'class_2'
//	#pragma warning(disable: 4577) // noexcept used with no exception handling mode
//	#pragma warning(disable: 4838) // conversion from enum to uint.
//	#pragma warning(disable: 4996) // sprintf unsafe
//	#pragma warning(disable: 4503) // Truncated decorated name
#endif

//} // namespace hub
