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
#        error "unable to import from build environment"
#        define SRC_API __declspec( dllimport )
#    endif

#else // unix
#    define SRC_API
#endif

#ifndef OS_WINDOWS

#    if ( __cplusplus >= 202001L )
#        define CPLUSPLUS_VERSION 20
#        define CONSTEXPR17 constexpr
#        define CONSTEXPR20 constexpr
#        define CONSTEXPR constexpr
#    elif ( __cplusplus >= 201703L )
#        define CPLUSPLUS_VERSION 17
#        define CONSTEXPR17 constexpr
#        define CONSTEXPR20
#        define CONSTEXPR constexpr
#    elif ( __cplusplus >= 201402L ) || ( defined( _MSC_VER ) && _MSC_VER >= 1914 )
#        define CPLUSPLUS_VERSION 14
#        define CONSTEXPR17
#        define CONSTEXPR20
#        define CONSTEXPR constexpr
#    else
#        define CPLUSPLUS_VERSION 14
#        define CONSTEXPR17
#        define CONSTEXPR20
// #    error "C++ version " __cplusplus " not supported"
#        define CONSTEXPR const
#    endif

#endif

// #define HUB_DEBUG_INPUT_OUTPUT

#ifdef HUB_DEBUG_INPUT_OUTPUT
#    define HUB_DEBUG_INPUT
#    define HUB_DEBUG_OUTPUT

// #    ifdef OS_LINUX
// #        define HUB_USE_BOOST
// #    endif
#endif

// filename = filename.substr( filename.find_last_of( '\\' ) + 1 );

// #include <string.h>

#ifdef WIN32
// #define FILE_NAME __FILE__
#    define FILE_NAME                                                   \
        std::string( __FILE__ )                                         \
            .substr( std::min( std::string( __FILE__ ).find_last_of( '\\' ), std::string(__FILE__).find_last_of( '/' )) + 1 )
            //.substr( std::string( __FILE__ ).find_last_of( '\\' ) + 1 ) \
            //.substr( std::string( __FILE__ ).find_last_of( '/' ) + 1 )

// #define FILE_NAME strrchr( __FILE__, '\\' ) ? strrchr( __FILE__, '\\' ) + 1 : __FILE__

// #define FILE_NAME std::string( strrchr( __FILE__, '\\' ) ? strrchr( __FILE__, '\\' ) + 1 :
// __FILE__ )

#else
#    define FILE_NAME std::string( __FILE_NAME__ )
#endif

#define FILE_NAME_WITHOUT_EXTENSION FILE_NAME.substr( 0, FILE_NAME.find_first_of( '.' ) )
//#define FILE_NAME_WITHOUT_EXTENSION FILE_NAME

#define HEADER                                                                             \
    "\033[" << std::to_string( 31 + reinterpret_cast<std::uintptr_t>( this ) % 7 ) << "m[" \
            << FILE_NAME_WITHOUT_EXTENSION << ":" << (uintptr_t)this % 100 << "]\033[0m "

// #if defined(OS_MACOS)
//     #define MAX_NET_BUFFER_SIZE 256'000
// #elif defined(OS_LINUX)
//     #define MAX_NET_BUFFER_SIZE 2'619'280 // 2.619280 Mo
// #elif defined(OS_WINDOWS)
//     #define MAX_NET_BUFFER_SIZE 2'000'000
// #endif

#ifdef WIN32
#    define MAX_STACK_SIZE 100'000
#else
#    define MAX_STACK_SIZE 100'000 // 100Ko
#endif

#include <iostream>
// #include <stdlib.h>
// #include <stdint.h>
#include <cstdint>

// #define HUB_USE_BOOST

#ifdef HUB_USE_BOOST
#    include <boost/type_index.hpp>
#endif

// template <class T>
// concept TypeNameable = requires( const T& t ) { t.name(); };

template <typename T>
concept isContainer = !std::is_same<T, std::string>() && requires( T t ) {
    std::begin( t );
    std::end( t );
};

//template <typename T>
//concept isPair = requires( T t ) {
//    t.first();
//    t.second();
//};

template <class T>
    requires( !isContainer<T> )
std::string type_name() {
    if constexpr ( requires { T::name(); } ) { return T::name(); }
    else if ( std::is_same_v<T, std::string> ) { return "string"; }
    else {
#ifdef HUB_USE_BOOST
        return boost::typeindex::type_id<typeof( T )>().pretty_name();
#else
        return typeid( T ).name();
#endif
    }
}

#include <vector>

template <class Container, class T = std::decay_t<decltype( *std::declval<Container>().begin() )>>
    requires( isContainer<Container> )
std::string type_name() {
    if constexpr ( std::is_same_v<Container, std::vector<T>> ) {
        return "vector<" + type_name<T>() + ">";
    }
    else { return "container<" + type_name<T>() + ">"; }
}

#define TYPE_NAME( Type ) type_name<Type>()

// #ifdef HUB_USE_BOOST
// #    define TYPE_NAME( t ) boost::typeindex::type_id<typeof( t )>().pretty_name()
// #else
// #    define TYPE_NAME( t ) typeid( t ).name()
// #endif

// #    ifdef HUB_USE_BOOST
//         std::cout << HEADER_INPUT_MSG "read(T) : <"
////                  << typeid( T ).name() << " ("
//                  << boost::typeindex::type_id<T>().pretty_name() << "> " << t  << std::endl;
// #    else
//        std::cout << typeid( T ).name() << "> = " << t
//                  << std::endl;
// #    endif

// #ifdef DEBUG
#include <cassert>
// #else
////#undef assert
// #endif

namespace hub {
using Data_t = unsigned char;
//    using Data_t =  std::byte;
using Size_t = uint64_t; // max = 18'446'744'073'709'551'616 ~= 18 exa
                         //    using Size_t = uint32_t;  // max = 4'294'967'296 ~= 4 giga
                         //    using Size_t = uint16_t; // max = 65'536 ~= 65 kilo
                         //    using Size_t = size_t;
                         //    using Size_t = int;
                         //    using Size_t = unsigned int;

template <class T>
constexpr auto sizeof_() {
    if constexpr ( requires { T::Size; } ) { return T::Size; }
    else { return sizeof( T ); }
}

template <class... Ts>
    requires( sizeof...( Ts ) > 1 )
constexpr auto sizeof_() {
    return ( sizeof_<Ts>() + ... );
}

// #ifdef HUB_DEBUG_INPUT
//     template <typename T>
//     void printTypeAndValue(const T & t) {
//     }

// #    ifdef HUB_USE_BOOST
//         std::cout << HEADER_INPUT_MSG "read(T) : <"
////                  << typeid( T ).name() << " ("
//                  << boost::typeindex::type_id<T>().pretty_name() << "> " << t  << std::endl;
// #    else
//        std::cout << typeid( T ).name() << "> = " << t
//                  << std::endl;
// #    endif
//    }
// #endif
} // namespace hub

/////////////////////////////////////////////// PRETTY BYTES
/////////////////////////////////////////////////

//#include <iostream>
#include <cmath>

//// source : https://www.mbeckler.org/blog/?p=114
//// Prints to the provided buffer a nice number of bytes (KB, MB, GB, etc)
static std::string pretty_bytes( hub::Size_t bytes ) {
    std::string str;

    constexpr auto buffSize = 32;
    char buff[buffSize] { 0 };
    //static const char* suffixes[7];
    static constexpr std::string_view suffixes[] {
        "Bytes", "KiloBytes", "MegaBytes", "GigaBytes", "TeraBytes", "PetaBytes", "ExaBytes" };
    //    suffixes[0]         = "Bytes";
    //    suffixes[1]         = "KiloBytes";
    //    suffixes[2]         = "MegaBytes";
    //    suffixes[3]         = "GigaBytes";
    //    suffixes[4]         = "TeraBytes";
    //    suffixes[5]         = "PetaBytes";
    //    suffixes[6]         = "ExaBytes";
    //    suffixes[0]         = "";
    //suffixes[0]         = " Bytes";
    //suffixes[1]         = " Ko";
    //suffixes[2]         = " Mo";
    //suffixes[3]         = " Go";
    //suffixes[4]         = " To";
    //suffixes[5]         = " Po";
    //suffixes[6]         = " Eo";
    uint64_t s          = 0; // which suffix to use
    double count        = bytes;
    constexpr auto kilo = 1'000;
    //    constexpr auto kilo = 1024;
    while ( count >= kilo && s < 7 ) {
        s++;
        count /= kilo;
    }
    //sprintf_s( buff, buffSize, "%d%s", (int)count, suffixes[s].data() );
    //return std::string( buff );

    if ( count - floor( count ) == 0.0 )
#ifdef WIN32
        //sprintf_s( buff, 80, "%d %s", (int)count, suffixes[s].data() );
        snprintf( buff, buffSize, "%d %s", (int)count, suffixes[s].data() );
#else
#    ifdef OS_MACOS
        snprintf( buff, buffSize, "%d %s", (int)count, suffixes[s].data() );
#    else
        sprintf( buff, "%d %s", (int)count, suffixes[s].data() );
#    endif
#endif
    else
#ifdef WIN32
        //sprintf_s( buff, 80, "%.1f %s", count, suffixes[s].data() );
        snprintf( buff, buffSize, "%.1f %s", count, suffixes[s].data() );
#else
#    ifdef OS_MACOS
        snprintf( buff, buffSize, "%.1f %s", count, suffixes[s].data() );
#    else
        sprintf( buff, "%.1f %s", count, suffixes[s].data() );
#    endif
#endif
    return std::string( buff );
}
#define PRETTY_BYTES( t ) pretty_bytes( t )

/////////////////////////////////////////////// STATIC WARNING
/////////////////////////////////////////////////

// #ifndef HUB_SERVICE_PORT
// #define HUB_SERVICE_PORT 4042
// #endif

// #if defined(__GNUC__)
// #define DEPRECATE(foo, msg) foo __attribute__((deprecated(msg)))
// #elif defined(_MSC_VER)
// #define DEPRECATE(foo, msg) __declspec(deprecated(msg)) foo
// #else
// #error This compiler is not supported
// #endif

// #define PP_CAT(x,y) PP_CAT1(x,y)
// #define PP_CAT1(x,y) x##y

// namespace detail
//{
// struct true_type {};
// struct false_type {};
// template <int test> struct converter : public true_type {};
// template <> struct converter<0> : public false_type {};
// }

//#define STATIC_WARNING(cond, msg) \
//struct PP_CAT(static_warning,__LINE__) { \
//        DEPRECATE(void _(::detail::false_type const& ),msg) {}; \
//        void _(::detail::true_type const& ) {}; \
//        PP_CAT(static_warning,__LINE__)() {_(::detail::converter<(cond)>());} \
//}

//// Note: using STATIC_WARNING_TEMPLATE changes the meaning of a program in a small way.
//// It introduces a member/variable declaration.  This means at least one byte of space
//// in each structure/class instantiation.  STATIC_WARNING should be preferred in any
//// non-template situation.
////  'token' must be a program-wide unique identifier.
//     #define STATIC_WARNING_TEMPLATE(token, cond, msg) \
//STATIC_WARNING(cond, msg) PP_CAT(PP_CAT(_localvar_, token),__LINE__)
