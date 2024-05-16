/*******************************************************************************
* Copyright (c) 2021 IRIT, computer science research laboratory, Toulouse, France.
* Please visit https://www.irit.fr/tplay/.
*
* All rights reserved.
* This code belongs to tplay/hub project (https://github.com/T-PLAY/hub).
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at https://mozilla.org/MPL/2.0/.
*
* Initial Contributors:
*   - Nicolas Mellado (IRIT)
*   - Gauthier Bouyjou (IRIT, Toulouse Tech Transfer)
*******************************************************************************/

#pragma once

namespace hub {

// Compiler identification
#if defined( __clang__ )
#    define COMPILER_CLANG
#    define CLANG_VERSION __clang_major__

#if (! defined(HUB_COMPILER_Clang)) && (! defined(HUB_COMPILER_AppleClang))
#error "not clang compiler"
#endif

#elif defined( __GNUC__ )
#    define COMPILER_GCC
#    include <features.h>
#    if __GNUC_PREREQ( 15, 0 )
#        define GCC_VERSION 15
#    elif __GNUC_PREREQ( 14, 0 )
#        define GCC_VERSION 14
#    elif __GNUC_PREREQ( 13, 0 )
#        define GCC_VERSION 13
#    elif __GNUC_PREREQ( 12, 0 )
#        define GCC_VERSION 12
#    elif __GNUC_PREREQ( 11, 0 )
#        define GCC_VERSION 11
#    elif __GNUC_PREREQ( 10, 0 )
#        define GCC_VERSION 10
#    elif __GNUC_PREREQ( 9, 0 )
#        define GCC_VERSION 9
#    elif __GNUC_PREREQ( 8, 0 )
#        define GCC_VERSION 8
#    else
#        error "gcc version not supported"
#    endif

#ifndef HUB_COMPILER_GNU
#error "not gnu compiler"
#endif

#elif defined( _MSC_VER )

//#if _MSC_VER >= 1900
#    define COMPILER_MSVC
#    define _USE_MATH_DEFINES

#ifndef HUB_COMPILER_MSVC
#error "not gnu compiler"
#endif

#elif defined( __llvm__ )

#ifndef HUB_COMPILER_LLVM
#error "not llvm compiler"
#endif

#else
#ifndef CPP_CHECK
#    error unsupported compiler
#endif
#endif

// OS and architecture identification
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
#ifndef CPP_CHECK
#    error unsupported OS
#endif
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

// Build configuration

// This tells apart between debug and release builds :
// DEBUG is defined in debug builds and RELEASE in release builds.
// Additionally REL_DEB is defined on release build with debug info
// Also the macro ON_DEBUG() can be used to execute an expression only on debug.
// By default, debug has assert macros enabled. In release builds

//// Make sure all "debug" macros are defined

// Explicit compiler warning disables.

#if defined( COMPILER_GCC )
#endif

#if defined( COMPILER_MSVC )
#    pragma warning( disable : 4244 ) // Conversion from double to float loses data.
#    pragma warning( disable : 4251 ) // stl dllexports
#    pragma warning( disable : 4267 ) // conversion from size_t to uint
#    pragma warning( disable : 4018 ) // signed/unsigned mismatch
// DLL-interface class 'class_2' 	#pragma warning(disable: 4577) // noexcept used with no
#    ifndef NOMINMAX
#        define NOMINMAX
#    endif
#endif

// Debug log message.
#define ENABLE_DEBUG_MSG

#if defined( ENABLE_DEBUG_MSG ) && defined( DEBUG )
#    define DEBUG_MSG( str )               \
        do {                               \
            std::cout << str << std::endl; \
        } while ( false )

#else
#    define DEBUG_MSG( str ) \
        do {                 \
        } while ( false )
#endif

// Dll import/export.
// You must define SRC_STATIC to force static link for external use (.lib)

//#ifndef SRC_STATIC
//#define SRC_STATIC
//#endif

#ifdef OS_WINDOWS
#    if defined SRC_STATIC
#        define SRC_API

#    elif defined SRC_IMPORTS
#        define SRC_API __declspec( dllimport )

#    elif defined SRC_EXPORTS
#        define SRC_API __declspec( dllexport )

#    else
#        error "unable to import from build environment"
#        define SRC_API __declspec( dllimport )
#    endif

#else // unix
#    define SRC_API
#endif

#ifdef WIN32
#    define CPLUSPLUS_VERSION _MSVC_LANG
#else
#    define CPLUSPLUS_VERSION __cplusplus
#endif

#if ( CPLUSPLUS_VERSION >= 202001L )
#    define CPP_VERSION 20
#    define CONSTEXPR17 constexpr
#    define CONSTEXPR20 constexpr
#    define CONSTEXPR constexpr
#elif ( CPLUSPLUS_VERSION >= 201703L )
#    define CPP_VERSION 17
#    define CONSTEXPR17 constexpr
#    define CONSTEXPR20
#    define CONSTEXPR constexpr
#elif ( CPLUSPLUS_VERSION >= 201402L )
#    define CPP_VERSION 14
#    define CONSTEXPR17
#    define CONSTEXPR20
#    define CONSTEXPR constexpr
#else
#    define CPP_VERSION 14
#    define CONSTEXPR17
#    define CONSTEXPR20
#    define CONSTEXPR const
#endif

// #define HUB_DEBUG_INPUT_OUTPUT
#ifdef HUB_DEBUG_INPUT_OUTPUT
#    define HUB_DEBUG_INPUT
#    define HUB_DEBUG_OUTPUT

#endif

#ifdef WIN32
#    define FILE_NAME                                                          \
        std::string( "/\\" __FILE__ )                                                \
            .substr( std::max( std::string( "/\\" __FILE__ ).find_last_of( '\\' ),   \
                               std::string( "/\\" __FILE__ ).find_last_of( '/' ) ) + \
                     1 )

#else // unix system
#    ifdef COMPILER_GCC

#        if GCC_VERSION < 12
#            define FILE_NAME                                                          \
                std::string( "/\\" __FILE__ )                                                \
                    .substr( std::max( std::string( "/\\" __FILE__ ).find_last_of( '\\' ),   \
                                       std::string( "/\\" __FILE__ ).find_last_of( '/' ) ) + \
                             1 )
#        else
#            define FILE_NAME std::string( __FILE_NAME__ )
#        endif

#    else
#        define FILE_NAME std::string( __FILE_NAME__ )
#    endif

#endif

#define FILE_NAME_WITHOUT_EXTENSION FILE_NAME.substr( 0, FILE_NAME.find_first_of( '.' ) )

#define HEADER                                                                             \
    "\033[" << std::to_string( 31 + reinterpret_cast<std::uintptr_t>( this ) % 7 ) << "m[" \
            << FILE_NAME_WITHOUT_EXTENSION << ":" << (uintptr_t)this % 100 << "]\033[0m "

#ifdef WIN32
#    define MAX_STACK_SIZE 100'000
#else
#    define MAX_STACK_SIZE 100'000 // 100Ko
#endif



} // namespace hub
