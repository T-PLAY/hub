
#pragma once

#include <fstream>

std::ofstream logFile;

#define DEBUG

#ifdef DEBUG
#    define DEBUG_MSG( str ) \
        do {                               \
            std::cout << str << std::endl; \
            if ( !logFile.is_open() ) { \
				logFile.open( "log.txt", std::ios_base::beg ); \
			} \
			assert( logFile.is_open() ); \
            logFile << str << std::endl; \
        } while ( false )
#else
#    define DEBUG_MSG( str ) \
        do {                 \
        } while (false )
#endif