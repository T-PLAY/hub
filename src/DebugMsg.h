
#pragma once

#include <fstream>

static std::ofstream s_logFile;

#define DEBUG

// if ( !s_logFile.is_open() ){ \
// 				s_logFile.open( "log.txt", std::ios_base::beg ); \
// 			} \
// 			assert( s_logFile.is_open() ); \
//             s_logFile << str << std::endl; \

#ifdef DEBUG
#    define DEBUG_MSG( str ) \
        do {                               \
            std::cout << str << std::endl; \
        } while ( false )
#else
#    define DEBUG_MSG( str ) \
        do {                 \
        } while (false )
#endif