#pragma once

//#include <fstream>
// static std::ofstream s_logFile;

namespace hub {

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
//#define SRC_EXPORTS

#ifdef WIN32
#    ifdef SRC_EXPORTS
#        define SRC_API __declspec( dllexport )
#    else
#        define SRC_API __declspec( dllimport )
#    endif

#else
#    define SRC_API
#endif

} // namespace hub
