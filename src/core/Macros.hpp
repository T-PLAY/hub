/// © 2021-2024 Hub, All Rights Reserved
/// @author gauthier <gauthierbouyjou@aol.com>
/// @date 2023/10/01
	
#pragma once

#include <cassert>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <map>
#include <tuple>
#include <type_traits>
#include <vector>

#include "Configuration.hpp"

// #define HUB_USE_BOOST
#ifdef HUB_USE_BOOST
#    include <boost/type_index.hpp>
#endif

namespace hub {

// ----------------------------------------------------------------------------
// Compiler identification
// ----------------------------------------------------------------------------
#if defined( __clang__ )
#    define COMPILER_CLANG
#define CLANG_VERSION __clang_major__

#elif defined( __GNUC__ )
#    define COMPILER_GCC
#include <features.h>
#  if __GNUC_PREREQ(15,0)
#define GCC_VERSION 15
#  elif __GNUC_PREREQ(14,0)
#define GCC_VERSION 14
#  elif __GNUC_PREREQ(13,0)
#define GCC_VERSION 13
#  elif __GNUC_PREREQ(12,0)
#define GCC_VERSION 12
#  elif __GNUC_PREREQ(11,0)
#define GCC_VERSION 11
#  elif __GNUC_PREREQ(10,0)
#define GCC_VERSION 10
#  elif __GNUC_PREREQ(9,0)
#define GCC_VERSION 9
#  elif __GNUC_PREREQ(8,0)
#define GCC_VERSION 8
#else
#error "gcc version not supported"
#endif

#elif defined( _MSC_VER )
#    define COMPILER_MSVC
#    define _USE_MATH_DEFINES
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

//#        define SRC_API __declspec( dllexport )

// #define CPP_VERSION 20

// #ifndef OS_WINDOWS
//#ifndef CPP_VERSION
#ifdef WIN32
    #define CPLUSPLUS_VERSION _MSVC_LANG
//    #ifdef __cplusplus
//#            error "__cplusplus may not found on win system"
//    #endif
    //#define __cplusplus _MSVC_LANG
#else
    #define CPLUSPLUS_VERSION __cplusplus
#endif

#    if ( CPLUSPLUS_VERSION >= 202001L )
#        define CPP_VERSION 20
#        define CONSTEXPR17 constexpr
#        define CONSTEXPR20 constexpr
#        define CONSTEXPR constexpr
#    elif ( CPLUSPLUS_VERSION >= 201703L )
#        define CPP_VERSION 17
#        define CONSTEXPR17 constexpr
#        define CONSTEXPR20
#        define CONSTEXPR constexpr
//#    elif ( CPLUSPLUS_VERSION >= 201402L ) || ( defined( _MSC_VER ) && _MSC_VER >= 1914 )
#    elif ( CPLUSPLUS_VERSION >= 201402L )
#        define CPP_VERSION 14
#        define CONSTEXPR17
#        define CONSTEXPR20
#        define CONSTEXPR constexpr
#    else
#        define CPP_VERSION 14
#        define CONSTEXPR17
#        define CONSTEXPR20
// #    error "C++ version " __cplusplus " not supported"
#        define CONSTEXPR const
#    endif

//#endif

// #define HUB_DEBUG_INPUT_OUTPUT

#ifdef HUB_DEBUG_INPUT_OUTPUT
#    define HUB_DEBUG_INPUT
#    define HUB_DEBUG_OUTPUT

// #    ifdef OS_LINUX
// #        define HUB_USE_BOOST
// #    endif
#endif

// filename = filename.substr( filename.find_last_of( '\\' ) + 1 );
// #include <features.h>

#ifdef WIN32
// #define FILE_NAME __FILE__
#    define FILE_NAME                                                          \
        std::string( __FILE__ )                                                \
            .substr( std::min( std::string( __FILE__ ).find_last_of( '\\' ),   \
                               std::string( __FILE__ ).find_last_of( '/' ) ) + \
                     1 )
//.substr( std::string( __FILE__ ).find_last_of( '\\' ) + 1 ) \
            //.substr( std::string( __FILE__ ).find_last_of( '/' ) + 1 )

// #define FILE_NAME strrchr( __FILE__, '\\' ) ? strrchr( __FILE__, '\\' ) + 1 : __FILE__

// #define FILE_NAME std::string( strrchr( __FILE__, '\\' ) ? strrchr( __FILE__, '\\' ) + 1 :
// __FILE__ )

#else // unix system
// #    define FILE_NAME std::string( __FILE_NAME__ )
#ifdef COMPILER_GCC

#if GCC_VERSION < 12
// #    define FILE_NAME std::string( __FILE__ )
#    define FILE_NAME                                                          \
        std::string( __FILE__ )                                                \
            .substr( std::min( std::string( __FILE__ ).find_last_of( '\\' ),   \
                               std::string( __FILE__ ).find_last_of( '/' ) ) + \
                     1 )
#else
#    define FILE_NAME std::string( __FILE_NAME__ )
#endif

#else
#    define FILE_NAME std::string( __FILE_NAME__ )
#endif

#endif

#define FILE_NAME_WITHOUT_EXTENSION FILE_NAME.substr( 0, FILE_NAME.find_first_of( '.' ) )
// #define FILE_NAME_WITHOUT_EXTENSION FILE_NAME

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

} // namespace hub

///////////////////////////////////////////////////////////////////////////////////

#if CPP_VERSION <= 14
#    include "traits/std_any.hpp"
#else
#    include <any>
#endif

namespace std {

#if CPP_VERSION < 20

template <typename T>
struct remove_cvref : std::remove_cv<std::remove_reference_t<T>> {};

template <typename T>
using remove_cvref_t = typename remove_cvref<T>::type;

#endif

//////////////////////////////////

#if CPP_VERSION < 17

template <class T, class U>
static constexpr auto is_same_v = is_same<T, U>::value;

template <class T>
static constexpr bool is_arithmetic_v = is_arithmetic<T>::value;

template <class T>
static constexpr bool is_array_v = is_array<T>::value;

#endif

#if CPP_VERSION <= 14 // std::void_t not supported by AppleClang (c++14)

using string_view = string;

template <class T>
static constexpr bool is_enum_v = is_enum<T>::value;

template <class T>
static constexpr bool is_pointer_v = is_pointer<T>::value;

template <class Base, class T>
static constexpr bool is_base_of_v = is_base_of<Base, T>::value;

#endif

#if defined( OS_MACOS ) && CPP_VERSION <= 14 // std::void_t not supported by AppleClang (c++14)
// namespace std {

template <typename... Ts>
struct make_void {
    typedef void type;
};

template <typename... Ts>
using void_t = typename make_void<Ts...>::type;

// } // namespace std
#endif

} // namespace std

///////////////////////////////////////////////////////////////////////////////////

namespace hub {

// template <class T>
// concept TypeNameable = requires( const T& t ) { t.name(); };

#if CPP_VERSION >= 20

template <typename T>
concept isContainer = !std::is_same<T, std::string>() && requires( T t ) {
    std::begin( t );
    std::end( t );
};

#else
template <typename T>
using has_begin_t = decltype( std::begin( std::declval<T>() ) );

template <typename T, typename = std::void_t<>>
struct has_begin : std::false_type {};

template <typename T>
struct has_begin<T, std::void_t<has_begin_t<T>>> : std::true_type {};

template <typename T>
static constexpr bool has_begin_v = has_begin<T>::value;

////////////

template <typename T>
using has_end_t = decltype( std::end( std::declval<T>() ) );

template <typename T, typename = std::void_t<>>
struct has_end : std::false_type {};

template <typename T>
struct has_end<T, std::void_t<has_end_t<T>>> : std::true_type {};

template <typename T>
static constexpr bool has_end_v = has_end<T>::value;
template <typename T>
constexpr bool isContainer = !std::is_same<T, std::string>() && has_begin_v<T> && has_end_v<T>;
#endif

static_assert( isContainer<std::vector<unsigned char>> );
static_assert( isContainer<std::vector<int>> );
static_assert( !isContainer<std::string> );
static_assert( !isContainer<const char*> );

// template <typename T>
// concept isPair = requires( T t ) {
//     t.first();
//     t.second();
// };

// #if CPP_VERSION >= 20

// template <class T>
//     requires( !isContainer<T> )
// std::string type_name() {
//     if constexpr ( requires { T::name(); } ) { return T::name(); }
//     else if ( std::is_same_v<T, std::string> ) { return "string"; }
//     else {
// #    ifdef HUB_USE_BOOST
//         return boost::typeindex::type_id<typeof( T )>().pretty_name();
// #    else
// #    endif
//     }
// }

// template <class Container, class T = std::decay_t<decltype( *std::declval<Container>().begin()
// )>>
//     requires( isContainer<Container> )
// std::string type_name() {
//     if constexpr ( std::is_same_v<Container, std::vector<T>> ) {
//         return "vector<" + type_name<T>() + ">";
//     }
//     else { return "container<" + type_name<T>() + ">"; }
// }

// // todo c++17
// template <class T>
// // requires( !isContainer<T> )
// // typename std::enable_if<isContainer<T>, std::string>
// std::string type_name() {
//     return "typename";
//     //     if constexpr ( requires { T::name(); } ) { return T::name(); }
//     //     else if ( std::is_same_v<T, std::string> ) { return "string"; }
//     //     else {
//     // #ifdef HUB_USE_BOOST
//     //         return boost::typeindex::type_id<typeof( T )>().pretty_name();
//     // #else
//     // #endif
//     // }
// }

// #else

////////////////////////////////////////////////////////// TYPE_NAME ///////////////////////////////////////

template <typename T>
using has_name_t = decltype( T::name() );

template <typename T, typename = std::void_t<>>
struct has_name : std::false_type {};

template <typename T>
struct has_name<T, std::void_t<has_name_t<T>>> : std::true_type {};

template <typename T>
static constexpr bool has_name_v = has_name<T>::value;

//////////////

template <typename T>
using nameable_t = decltype( name( std::declval<T>() ) );

template <typename T, typename = std::void_t<>>
struct nameable : std::false_type {};

template <typename T>
struct nameable<T, std::void_t<nameable_t<T>>> : std::true_type {};

template <typename T>
static constexpr bool nameable_v = nameable<T>::value;

// template <class T>
// // typename std::enable_if_t<!has_name_v<T> && std::is_same_v<T, std::string>, std::string>
// std::string type_name(const T & t) {
//     return "string";
// }

// template <class T>
// std::string type_name() {
//     return "string";
// }

// template <class T>
// typename std::enable_if_t<has_name_v<std::remove_cv_t<T>>, std::string>

// template <class T>
// static std::string typeName( const int& ) {
// static typename std::enable_if_t<std::is_same_v<T, int>, std::string> t

template <class T>
// static typename std::enable_if_t<!has_name_v<std::remove_cv_t<T>>, std::string>
static typename std::enable_if_t<! nameable_v<T> && !has_name_v<T>, std::string> typeName( const T& t ) {
// typeName(  ) {
#ifdef HUB_USE_BOOST
    return boost::typeindex::type_id<typeof( T )>().pretty_name();
#else
    return typeid( T ).name();
#endif
    // return "unreconized";
}

template <class T>
// static typename std::enable_if_t<has_name_v<std::remove_cv_t<T>>, std::string>
static typename std::enable_if_t<! nameable_v<T> && has_name_v<T>, std::string> typeName( const T& t ) {
    // typeName( ) {
    return T::name();
}

template <class T>
// static typename std::enable_if_t<has_name_v<std::remove_cv_t<T>>, std::string>
static typename std::enable_if_t<nameable_v<T>, std::string> typeName( const T& t ) {
    static_assert(! has_name_v<T>);
    // typeName( ) {
    return name(t);
}

static std::string typeName( const std::string& ) {
    return "string";
}
static std::string   typeName( int ) {
     return "int";
}
 static std::string typeName( double) {
     return "double";
 }
 static std::string typeName( bool ) {
     return "bool";
 }
 static std::string typeName( float ) {
     return "float";
 }
 static std::string typeName( char ) {
 return "char";
 }
 static std::string typeName( unsigned char ) {
     return "uchar";
 }
 static std::string typeName( const char* ) {
     return "cstr";
 }
 static std::string typeName( long long ) {
     return "long long";
 }


// static auto typeName(void) {
//     return "void";
// }
template <class T>
static std::string typeName( const std::vector<T>& ) {
    // return "vector<" + typeName( T() ) + ">";
    // return "vector<" + typeName<T>() + ">";
    // return "vector<" + typeName( std::declval<T>() ) + ">";
    return "vector<" + typeName( T() ) + ">";
}

template <class First, class Second>
static std::string typeName( const std::pair<First, Second>& ) {
    // return "vector<" + typeName( T() ) + ">";
    // return "pair<" + typeName<First>() + ", " + typeName<Second>() + ">";
    // return "pair<" + typeName( std::declval<First>() ) + ", " + typeName( std::declval<Second>()
    // ) +
    // ">";
    return "pair<" + typeName( First() ) + ", " + typeName( Second() ) + ">";
}

template <class Key, class Value>
static std::string typeName( const std::map<Key, Value>& ) {
    // return "map<" + typeName<Key>() + ", " + typeName<Value>() + ">";
    // return "map<" + typeName( std::declval<Key>() ) + ", " + typeName( std::declval<Value>() ) +
    // ">";
    return "map<" + typeName( Key() ) + ", " + typeName( Value() ) + ">";
}

template <class T, class... Ts>
static void typeName_recurse( std::string& str ) {
    str += ", " + typeName( T() );
    if CONSTEXPR17 ( sizeof...( Ts ) > 0 ) { typeName_recurse<Ts...>( str ); }
}

template <class T, class... Ts>
static std::string typeName( const std::tuple<T, Ts...>& ) {
    std::string str = "tuple<";
    str += typeName( T() );
#if CPP_VERSION > 14
    if CONSTEXPR17 ( sizeof...( Ts ) > 0 ) { typeName_recurse<Ts...>( str ); }
#endif
    str += ">";
    return str;
    // return "map<" + typeName( Key() ) + ", " + typeName( Value() ) +
    // ">";
}

template <class T, class... Ts>
static std::string typeName() {
    std::string str;
    str += typeName( T() );
    if constexpr ( sizeof...( Ts ) > 0 ) { str += ", " + typeName<Ts...>(); }
    return str;
}

// template <class T>
// typename std::enable_if_t<has_name_v<std::remove_cv_t<T>>, std::string> type_name() {
//     return T::name();
// }

// template <class T>
// typename std::enable_if_t<std::is_same_v<std::remove_cv_t<T>, void>, std::string> type_name() {
//     return "void";
// }
// template <class T>
// typename std::enable_if_t<std::is_same_v<std::remove_cv_t<T>, double>, std::string> type_name() {
//     return "double";
// }
// template <class T>
// typename std::enable_if_t<std::is_same_v<std::remove_cv_t<T>, unsigned char>, std::string>
// type_name() {
//     return "uchar";
// }
// template <class T>
// typename std::enable_if_t<std::is_same_v<std::remove_cv_t<T>, int>, std::string> type_name() {
//     return "int";
// }
// template <class T>
// typename std::enable_if_t<std::is_same_v<T, const char*>, std::string> type_name() {
//     return "cstr";
// }
// template <class T>
// typename std::enable_if_t<std::is_same_v<std::remove_cv_t<T>, std::string>, std::string>
// type_name() {
//     return "string";
// }
// // template <class T>
// // typename std::enable_if_t<std::is_same_v<T, const std::string>, std::string> type_name() {
// // return "const string";
// // }
// // template <class T, class U>
// // static auto type_name( const std::map<T, U>& cont ) {
// //     return "map";
// // }
// template <class Pair,
//           class Key   = std::decay_t<decltype( std::declval<Pair>().first )>,
//           class Value = std::decay_t<decltype( std::declval<Pair>().second )>>
// typename std::enable_if_t<std::is_same_v<Pair, std::pair<Key, Value>>, std::string> type_name() {
//     // std::string type_name() {
//     return "pair<" + type_name<Key>() + ", " + type_name<Value>() + ">";
// }

// // template <class Container, class T = std::decay_t<decltype( *std::declval<Container>().begin()
// template <class Map,
//           class Key   = std::decay_t<decltype( std::declval<Map>().begin()->first )>,
//           class Value = std::decay_t<decltype( std::declval<Map>().begin()->second )>>
// typename std::enable_if_t<std::is_same_v<Map, std::map<Key, Value>>, std::string> type_name() {
//     // std::string type_name() {
//     return "map<" + type_name<Key>() + ", " + type_name<Value>() + ">";
// }

// template <class Vector, class T = std::decay_t<decltype( *std::declval<Vector>().begin() )>>
// // requires( isContainer<Vector> )
// typename std::enable_if_t<std::is_same_v<Vector, std::vector<T>>, std::string> type_name() {
//     // std::string type_name() {
//     return "vector<" + type_name<T>() + ">";
//     // if constexpr ( std::is_same_v<Vector, std::vector<T>> ) {
//     // return "vector<" + type_name<T>() + ">";
//     // }
//     // else { return "container<" + type_name<T>() + ">"; }
// }

// template <class Tuple, class T = std::decay_t<std::remove_cvref_t<decltype( std::get<0>(
// std::declval<Tuple>() ) )>>>
// template <class Tuple>
// typename std::enable_if_t<std::tuple_size_v<Tuple> == 1, std::string> type_name() {
//     return "tuple<" + type_name<decltype( std::get<0>( Tuple() ) )>() + ">";
// }

// template <
//     class Tuple,
//     class T = std::decay_t<std::remove_cvref_t<decltype( std::get<0>( std::declval<Tuple>() )
//     )>>, class U = std::decay_t<std::remove_cvref_t<decltype( std::get<1>( std::declval<Tuple>()
//     ) )>>>
// typename std::enable_if_t<std::tuple_size_v<Tuple> == 2 && std::is_same_v<Tuple, std::tuple<T,
// U>>,
//                           std::string>
// type_name() {
//     return "tuple<" + type_name<T>() + ", " + type_name<U>() + ">";
// }

// template <class Tuple,
//           class T = std::decay_t<std::remove_cvref_t<decltype( std::get<0>( std::declval<Tuple>()
//           ) )>>, class U = std::decay_t<decltype( std::get<1>( std::declval<Tuple>() ) )>, class
//           V = std::decay_t<decltype( std::get<2>( std::declval<Tuple>() ) )>>
// typename std::enable_if_t<std::is_same_v<Tuple, std::tuple<T, U, V>>, std::string> type_name() {
//     return "tuple<" + type_name<T>() + ", " + type_name<U>() + ", " + type_name<V>() + ">";
// }

// template <class T>
// typename std::enable_if_t<!has_name_v<T> && std::is_same_v<T, std::string>, std::string>
// type_name(const T & t) {
//     return "string";
// }

// template <class T>
// typename std::enable_if_t<!has_name_v<T> && !std::is_same_v<T, std::string>, std::string>
// type_name() {
// #        ifdef HUB_USE_BOOST
//     return boost::typeindex::type_id<typeof( T )>().pretty_name();
// #        else
// #        endif
// }

// template <class Container, class T = std::decay_t<decltype( *std::declval<Container>().begin()
// )>>
//     requires( isContainer<Container> )
// std::string type_name() {
//     if constexpr ( std::is_same_v<Container, std::vector<T>> ) {
//         return "vector<" + type_name<T>() + ">";
//     }
//     else { return "container<" + type_name<T>() + ">"; }
// }

// #endif

// #define TYPE_NAME( Type ) hub::type_name<Type>()
#define TYPE_NAME( _Type_ ) hub::typeName( _Type_ )

/////////////////////////////////////////// TYPE_ID ////////////////////////////////////////////////

using TypeId_t = int;

template <typename T>
using has_id_t = decltype( T::id );

template <typename T, typename = std::void_t<>>
struct has_id : std::false_type {};

template <typename T>
struct has_id<T, std::void_t<has_id_t<T>>> : std::true_type {};

template <typename T>
static constexpr bool has_id_v = has_id<T>::value;

namespace Types {
enum Cpp : TypeId_t {
    INT = 0,
    BOOL,
    Cpp_Count
};
}
static_assert(Types::Cpp_Count == 2);

template <class T>
static constexpr typename std::enable_if_t<has_id_v<T>, TypeId_t>
typeId( const T& t ) {
    // return (TypeId_t)T::id;
    return T::id;
}

template <class T>
static constexpr typename std::enable_if_t<! has_id_v<T>, TypeId_t>
typeId( const T& t ) {
    // return (TypeId_t)T::id;
    return typeid(T).hash_code();
}

// template <class T>
// // auto typeId( const T& t ) -> std::size_t {
// constexpr auto typeId(const T & t) -> TypeId_t {
//     // return typeid(T).hash_code();
//     // return 0;
//     return std::hash<std::string>()( typeName( T() ) );
// }

constexpr auto typeId(int) -> TypeId_t {
    return Types::Cpp::INT;
}

constexpr auto typeId(bool) -> TypeId_t {
    return Types::Cpp::BOOL;
}

// #define TYPE_ID( _Type_ ) hub::typeId<_Type_>()
#define TYPE_ID( _Type_ ) hub::typeId(_Type_())

#if CPP_VERSION >= 20
// #    define REQUIRES( _Type_, _COND_ )
#    define REQUIRES( _CONST_, _COND_, _TYPE_ ) requires( _COND_ ) _CONST_ _TYPE_
// #    define REQUIRES( _COND_, _TYPE_ ) requires( _COND_ ) _TYPE_
#else
// typename std::enable_if<( sizeof...( Types ) > 1 ), bool>::type
// #    define REQUIRES( _COND_, _TYPE_ ) typename std::enable_if<( _COND_ ), _TYPE_>::type
#    define REQUIRES( _CONST_, _COND_, _TYPE_ ) \
        _CONST_ typename std::enable_if_t<( _COND_ ), _TYPE_>
#endif
// #define TYPE_NAME_T( _Type_ ) hub::typeName(_Type_())

// #define TYPE_NAME( Type ) hub::type_name(std::declval<Type>())

// #ifdef HUB_USE_BOOST
// #    define TYPE_NAME( t ) boost::typeindex::type_id<typeof( t )>().pretty_name()
// #else
// #endif

// #    ifdef HUB_USE_BOOST
//         std::cout << HEADER_INPUT_MSG "read(T) : <"
//                  << boost::typeindex::type_id<T>().pretty_name() << "> " << t  << std::endl;
// #    else
//                  << std::endl;
// #    endif

// #ifdef DEBUG
// #else
////#undef assert
// #endif

// namespace hub {
using Data_t  = unsigned char;
using Datas_t = std::vector<Data_t>;
//    using Data_t =  std::byte;
using Size_t = uint64_t; // max = 18'446'744'073'709'551'616 ~= 18 exa
                         //    using Size_t = uint32_t;  // max = 4'294'967'296 ~= 4 giga
                         //    using Size_t = uint16_t; // max = 65'536 ~= 65 kilo
                         //    using Size_t = size_t;
                         //    using Size_t = int;
                         //    using Size_t = unsigned int;

#if CPP_VERSION >= 20
template <class T>
constexpr auto sizeOf() {
    if constexpr ( requires { T::Size; } ) { return T::Size; }
    else { return sizeof( T ); }
}

template <class... Ts>
    requires( sizeof...( Ts ) > 1 )
constexpr auto sizeOf() {
    return ( sizeOf<Ts>() + ... );
}

#else

template <typename T>
using has_Size_t = decltype( T::Size );

template <typename T, typename = std::void_t<>>
struct has_Size : std::false_type {};

template <typename T>
struct has_Size<T, std::void_t<has_Size_t<T>>> : std::true_type {};

template <typename T>
static constexpr bool has_Size_v = has_Size<T>::value;

////////////////////////

template <typename T>
using has_size_t = decltype( std::declval<T>().size() );

template <typename T, typename = std::void_t<>>
struct has_size : std::false_type {};

template <typename T>
struct has_size<T, std::void_t<has_size_t<T>>> : std::true_type {};

template <typename T>
static constexpr bool has_size_v = has_size<T>::value;

// todo c++17
// template <class T>
// constexpr auto sizeOf() {
// if constexpr ( requires { T::Size; } ) { return T::Size; }
//     if CONSTEXPR17 ( has_Size_v<T> ) { return T::Size; }
//     else { return sizeof( T ); };
// }
template <class T>
REQUIRES( static constexpr, has_Size_v<T>, decltype( sizeof( int ) ) )
sizeOf() {
    return T::Size;
}
template <class T>
REQUIRES( static constexpr, !has_Size_v<T>, decltype( sizeof( int ) ) )
sizeOf() {
    return sizeof( T );
}

// #if CPP_VERSION >= 17
template <class... Ts>
// requires( sizeof...( Ts ) > 1 )
REQUIRES( constexpr, sizeof...( Ts ) > 1, decltype( sizeof( int ) ) ) sizeOf() {
    static_assert( sizeof...( Ts ) > 1 );
    return ( sizeOf<Ts>() + ... );
}

template <class T>
REQUIRES( static constexpr, has_size_v<T>, decltype( sizeof( int ) ) )
sizeOf( const T& t ) {
    // static_assert(isMatrix<T>);
    return t.size();
    // return T::Size;
}

template <class T, class... Ts>
// requires( sizeof...( Ts ) > 1 )
REQUIRES( constexpr, sizeof...( Ts ) > 0, decltype( sizeof( int ) ) )
    sizeOf( const T& t, const Ts&... ts ) {

    if constexpr ( sizeof...( Ts ) > 0 ) { return sizeOf( t ) + sizeOf( ts... ); }
    else { return sizeOf( t ); }
    // static_assert( sizeof...( Ts ) > 1 );
    // return ( sizeOf(ts) + ... );
}
// #endif

#endif

// #ifdef HUB_DEBUG_INPUT
//     template <typename T>
//     void printTypeAndValue(const T & t) {
//     }

// #    ifdef HUB_USE_BOOST
//         std::cout << HEADER_INPUT_MSG "read(T) : <"
//                  << boost::typeindex::type_id<T>().pretty_name() << "> " << t  << std::endl;
// #    else
//                  << std::endl;
// #    endif
//    }
// #endif
// } // namespace hub

/////////////////////////////////////////////// PRETTY BYTES
/////////////////////////////////////////////////

//// source : https://www.mbeckler.org/blog/?p=114
//// Prints to the provided buffer a nice number of bytes (KB, MB, GB, etc)
static std::string pretty_bytes( hub::Size_t bytes ) {
    std::string str;

    constexpr auto buffSize = 32;
    char buff[buffSize] { 0 };
    // static const char* suffixes[7];
    //  static constexpr std::string_view suffixes[] {
    //  "Bytes", "KiloBytes", "MegaBytes", "GigaBytes", "TeraBytes", "PetaBytes", "ExaBytes" };
    static CONSTEXPR17 std::string_view suffixes[] { "Bytes", "Ko", "Mo", "Go", "To", "Po", "Eo" };
    //    suffixes[0]         = "Bytes";
    //    suffixes[1]         = "KiloBytes";
    //    suffixes[2]         = "MegaBytes";
    //    suffixes[3]         = "GigaBytes";
    //    suffixes[4]         = "TeraBytes";
    //    suffixes[5]         = "PetaBytes";
    //    suffixes[6]         = "ExaBytes";
    //    suffixes[0]         = "";
    // suffixes[0]         = " Bytes";
    // suffixes[1]         = " Ko";
    // suffixes[2]         = " Mo";
    // suffixes[3]         = " Go";
    // suffixes[4]         = " To";
    // suffixes[5]         = " Po";
    // suffixes[6]         = " Eo";
    uint64_t s          = 0; // which suffix to use
    double count        = bytes;
    constexpr auto kilo = 1'000;
    //    constexpr auto kilo = 1024;
    while ( count >= kilo && s < 7 ) {
        s++;
        count /= kilo;
    }
    // sprintf_s( buff, buffSize, "%d%s", (int)count, suffixes[s].data() );
    // return std::string( buff );

    if ( count - floor( count ) == 0.0 )
#ifdef WIN32
        // sprintf_s( buff, 80, "%d %s", (int)count, suffixes[s].data() );
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
        // sprintf_s( buff, 80, "%.1f %s", count, suffixes[s].data() );
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
#define PRETTY_BYTES( t ) hub::pretty_bytes( t )

/////////////////////////////////////////////// STATIC WARNING
/////////////////////////////////////////////////

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
//                                     #define STATIC_WARNING_TEMPLATE(token, cond, msg) \
//STATIC_WARNING(cond, msg) PP_CAT(PP_CAT(_localvar_, token),__LINE__)

} // namespace hub
