

#pragma once


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
