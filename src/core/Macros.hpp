#pragma once

// ----------------------------------------------------------------------------
// Compiler identification
// ----------------------------------------------------------------------------
#if defined( __GNUC__ )
#    define COMPILER_GCC
#elif defined( _MSC_VER )
#    define COMPILER_MSVC
#    define _USE_MATH_DEFINES
#elif defined( __clang__ )
#    define COMPILER_CLANG
#else
#    error unsupported compiler
#endif

// ----------------------------------------------------------------------------
// OS and architecture identification
// ----------------------------------------------------------------------------

#if defined( _WIN32 ) || defined( _WIN64 ) // ------------------------------ Windows
                                           // Shlwapi.h also define this macro
#    ifndef OS_WINDOWS
#        define OS_WINDOWS
#    endif
#    if defined( _M_X64 ) || defined( __x86_64__ )
#        define ARCH_X64
#    elif defined( i386 ) || defined( __i386__ ) || defined( __i386 ) || defined( _M_IX86 )
#        define ARCH_X86
#    elif defined( __arm__ ) || defined( __arm ) || defined( _M_ARM )
#        define ARCH_ARM32
#    elif defined( __aarch64__ ) || defined( _M_ARM64 )
#        define ARCH_ARM64
#    else
#        error unsupported arch
#    endif
#elif defined( __APPLE__ ) || defined( __MACH__ ) // ------------------------ Mac OS
#    define OS_MACOS
#elif defined( __linux__ ) || defined( __CYGWIN__ ) // ---------------------- Linux
#    define OS_LINUX
#else
#    error unsupported OS
#endif

// Check arch for macos and linux
#if defined( OS_MACOS ) || defined( OS_LINUX )
#    if defined( __i386__ )
#        define ARCH_X86
#    elif defined( __x86_64__ ) || defined( __x86_64 )
#        define ARCH_X64
#    elif defined( __arm__ ) || defined( __arm )
#        define ARCH_ARM32
#    elif defined( __aarch64__ ) || defined( __aarch64 )
#        define ARCH_ARM64
#    else
#        error unsupported arch
#    endif
#endif

// Todo : endianness, pointer sixe

// ----------------------------------------------------------------------------
// Build configuration
// ----------------------------------------------------------------------------

// This tells apart between debug and release builds :
// DEBUG is defined in debug builds and RELEASE in release builds.
// Additionally REL_DEB is defined on release build with debug info
// Also the macro ON_DEBUG() can be used to execute an expression only on debug.
// By default, debug has assert macros enabled. In release builds
// asserts are disabled except if explicitly required by
// defining CORE_USE_ASSERT

// #define DEBUG

//// Make sure all "debug" macros are defined
// #if defined (DEBUG) || defined(_DEBUG) || defined (CORE_DEBUG) // ------- Debug
// #   undef CORE_DEBUG
// #   define CORE_DEBUG

// #   undef _DEBUG
// #   define _DEBUG

// #   undef DEBUG
// #   define DEBUG

// #   undef NDEBUG
// #   undef RELEASE

// #   define ON_DEBUG(CODE) CODE
// #else // --------------------------------------------------------------- Release

// #   define RELEASE

// #ifndef NO_DEBUG_INFO
// #   define REL_DEB
// #endif

// #   undef CORE_DEBUG
// #   undef DEBUG
// #   undef _DEBUG

// #   if !defined (NDEBUG)
// #       define NDEBUG
// #   endif

// #   define ON_DEBUG(CODE) /* Nothing */
// #endif

// ----------------------------------------------------------------------------
// Explicit compiler warning disables.
// ----------------------------------------------------------------------------

#if defined( COMPILER_GCC )
#endif
#if defined( COMPILER_MSVC )
#    pragma warning( disable : 4244 ) // Conversion from double to float loses data.
#    pragma warning( disable : 4251 ) // stl dllexports
#    pragma warning( disable : 4267 ) // conversion from size_t to uint
#    pragma warning( disable : 4018 ) // signed/unsigned mismatch
// DLL-interface class 'class_2' 	#pragma warning(disable: 4577) // noexcept used with no
// exception handling mode 	#pragma warning(disable: 4838) // conversion from enum to uint.
// #pragma warning(disable: 4996) // sprintf unsafe 	#pragma warning(disable: 4503) // Truncated
// decorated name
#    ifndef NOMINMAX
#        define NOMINMAX
#    endif
#endif

// ----------------------------------------------------------------------------
// Debug log message.
// ----------------------------------------------------------------------------


// #if SRC_EXPORTS
#define ENABLE_DEBUG_MSG
// #endif

#if defined( ENABLE_DEBUG_MSG ) && defined( DEBUG )
#    define DEBUG_MSG( str )               \
        do {                               \
            std::cout << str << std::endl; \
        } while ( false )
// std::cout << "[" << __FILE_NAME__ << "][" << __FUNCTION__ << ":" << __LINE__ << "] " << str <<
// std::endl;

#else
#    define DEBUG_MSG( str ) \
        do {                 \
        } while ( false )
#endif

// ----------------------------------------------------------------------------
// Dll import/export.
// ----------------------------------------------------------------------------

// #ifndef SRC_STATIC
// #define SRC_STATIC // force static link for external use
// #endif

// #undef SRC_EXPORTS

#ifdef OS_WINDOWS
#    if defined SRC_STATIC
// todo fix unable to link static lib from external project, only single dll (import) is possible
#        define SRC_API
// #undef SRC_EXPORTS

#    elif defined SRC_EXPORTS
#        define SRC_API __declspec( dllexport )

#    else
#    error "unable to import from build environment"
#        define SRC_API __declspec( dllimport )
#    endif

#else // unix
#    define SRC_API
#endif

#if ( __cplusplus >= 202001L )
#    define CPLUSPLUS_VERSION 20
#    define CONSTEXPR17 constexpr
#    define CONSTEXPR20 constexpr
#    define CONSTEXPR constexpr
#elif ( __cplusplus >= 201703L )
#    define CPLUSPLUS_VERSION 17
#    define CONSTEXPR17 constexpr
#    define CONSTEXPR20
#    define CONSTEXPR constexpr
#elif ( __cplusplus >= 201402L ) || ( defined( _MSC_VER ) && _MSC_VER >= 1914 )
#    define CPLUSPLUS_VERSION 14
#    define CONSTEXPR17
#    define CONSTEXPR20
#    define CONSTEXPR constexpr
#else
#    define CPLUSPLUS_VERSION 14
#    define CONSTEXPR17
#    define CONSTEXPR20
// #    error "C++ version " __cplusplus " not supported"
#define CONSTEXPR const
#endif

//#define DEBUG_INPUT_OUTPUT

#ifdef DEBUG_INPUT_OUTPUT
#    define HUB_DEBUG_INPUT
#    define HUB_DEBUG_OUTPUT

#    ifdef OS_LINUX
#        define HUB_USE_BOOST
#    endif
#endif


//filename = filename.substr( filename.find_last_of( '\\' ) + 1 );

//#include <string.h>

#ifdef WIN32
//#define FILE_NAME __FILE__
//#    define FILE_NAME std::string( __FILE__ ).substr( std::string(__FILE__).find_last_of( '\\' ) + 1 )

//#define FILE_NAME strrchr( __FILE__, '\\' ) ? strrchr( __FILE__, '\\' ) + 1 : __FILE__

#define FILE_NAME std::string( strrchr( __FILE__, '\\' ) ? strrchr( __FILE__, '\\' ) + 1 : __FILE__ )


#else
#define FILE_NAME std::string(__FILE_NAME__)
#endif

//#if defined(OS_MACOS)
//    #define MAX_NET_BUFFER_SIZE 256'000
//#elif defined(OS_LINUX)
//    #define MAX_NET_BUFFER_SIZE 2'619'280 // 2.619280 Mo
//#elif defined(OS_WINDOWS)
//    #define MAX_NET_BUFFER_SIZE 2'000'000
//#endif

#include <iostream>
//#include <stdlib.h>
//#include <stdint.h>
#include <cstdint>

//#define HUB_USE_BOOST

#ifdef HUB_USE_BOOST
#    include <boost/type_index.hpp>
#endif


#ifdef HUB_USE_BOOST
    #define TYPE_NAME(t) \
        boost::typeindex::type_id<typeof(t)>().pretty_name()
#else
    #define TYPE_NAME(t) \
        typeid(t).name()
#endif
//#    ifdef HUB_USE_BOOST
//        std::cout << HEADER_INPUT_MSG "read(T) : <"
////                  << typeid( T ).name() << " ("
//                  << boost::typeindex::type_id<T>().pretty_name() << "> " << t  << std::endl;
//#    else
//        std::cout << typeid( T ).name() << "> = " << t
//                  << std::endl;
//#    endif

//#ifdef DEBUG
#include <cassert>
//#else
////#undef assert
//#endif

namespace hub {
    using Data_t = unsigned char;
//    using Size_t = uint64_t;
    using Size_t = uint32_t;
//    using Size_t = uint16_t;
//    using Size_t = size_t;
//    using Size_t = int;
//    using Size_t = unsigned int;

//#ifdef HUB_DEBUG_INPUT
//    template <typename T>
//    void printTypeAndValue(const T & t) {
//    }

//#    ifdef HUB_USE_BOOST
//        std::cout << HEADER_INPUT_MSG "read(T) : <"
////                  << typeid( T ).name() << " ("
//                  << boost::typeindex::type_id<T>().pretty_name() << "> " << t  << std::endl;
//#    else
//        std::cout << typeid( T ).name() << "> = " << t
//                  << std::endl;
//#    endif
//    }
//#endif
}
