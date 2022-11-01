# * try to find libusb-1 library
#
# Cache Variables: (probably not for direct use in your scripts) LIBUSB1_LIBRARY LIBUSB1_INCLUDE_DIR
#
# Non-cache variables you should use in your CMakeLists.txt: LIBUSB1_LIBRARIES LIBUSB1_INCLUDE_DIRS
# LIBUSB1_FOUND - if this is not true, do not attempt to use this library
#
# Requires these CMake modules: ProgramFilesGlob FindPackageHandleStandardArgs (known included with
# CMake >=2.6.2)
#
# Original Author: 2009-2010 Ryan Pavlik <rpavlik@iastate.edu> <abiryan@ryand.net>
# http://academic.cleardefinition.com Iowa State University HCI Graduate Program/VRAC
#
# Copyright Iowa State University 2009-2010. Distributed under the Boost Software License, Version
# 1.0. (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
# set(LIBUSB1_ROOT_DIR "${LIBUSB1_ROOT_DIR}" CACHE PATH "Root directory to search for libusb-1")
#
# if(WIN32) include(ProgramFilesGlob) program_files_fallback_glob(_dirs "LibUSB-Win32")
# if(CMAKE_SIZEOF_VOID_P EQUAL 8) if(MSVC) set(_lib_suffixes lib/msvc_x64 MS64/static) endif()
# else() if(MSVC) set(_lib_suffixes lib/msvc MS32/static) elseif(COMPILER_IS_GNUCXX)
# set(_lib_suffixes lib/gcc) endif() endif() else() set(_lib_suffixes) find_package(PkgConfig QUIET)
# if(PKG_CONFIG_FOUND) pkg_check_modules(PC_LIBUSB1 libusb-1.0) endif() endif()
#
# find_path(LIBUSB1_INCLUDE_DIR NAMES libusb.h PATHS ${PC_LIBUSB1_INCLUDE_DIRS}
# ${PC_LIBUSB1_INCLUDEDIR} ${_dirs} HINTS "${LIBUSB1_ROOT_DIR}" PATH_SUFFIXES include/libusb-1.0
# include libusb-1.0)
#
# find_library(LIBUSB1_LIBRARY NAMES libusb-1.0 usb-1.0 PATHS ${PC_LIBUSB1_LIBRARY_DIRS}
# ${PC_LIBUSB1_LIBDIR} ${_dirs} HINTS "${LIBUSB1_ROOT_DIR}" PATH_SUFFIXES ${_lib_suffixes})
#
# add_library(Libusb1 STATIC ${LIBUSB1_INCLUDE_DIR}/libusb.h)
#
# add_library(Libusb1 INTERFACE IMPORTED)
#
# include(FindPackageHandleStandardArgs) find_package_handle_standard_args(Libusb1 DEFAULT_MSG
# REQUIRED_VARS LIBUSB1_LIBRARY LIBUSB1_INCLUDE_DIR)
#
# if(LIBUSB1_FOUND) set(LIBUSB1_LIBRARIES "${LIBUSB1_LIBRARY}") set(LIBUSB1_INCLUDE_DIRS
# "${LIBUSB1_INCLUDE_DIR}")
#
# mark_as_advanced(LIBUSB1_ROOT_DIR)
#
# set_target_properties(Libusb1 PROPERTIES INTERFACE_LINK_LIBRARIES ${LIBUSB1_LIBRARY}
# INTERFACE_INCLUDE_DIRECTORIES ${LIBUSB1_INCLUDE_DIR} )
#
# endif()
#
# mark_as_advanced(LIBUSB1_INCLUDE_DIR LIBUSB1_LIBRARY)
#

# set(LIBUSB_ROOT_DIR "<UNSET>" CACHE PATH "Path of libusb dir on your system") if (LIBUSB_ROOT_DIR
# STREQUAL "<UNSET>") message(FATAL_ERROR "You must set LIBUSB_ROOT_DIR path of libusb folder in
# your system see :
# https://github.com/libusb/libusb/releases/download/v1.0.26/libusb-1.0.26-binaries.7z") endif()
#
# #find_path(LIBUSB_INCLUDE_DIR #    NAMES LIBUSB.h #    PATHS ${LIBUSB_ROOT_DIR}/Inc #    DOC
# "LIBUSB include path" #    )
#
# if (WIN32) file(GLOB LIBUSB_DLL ${LIBUSB_ROOT_DIR}/dll/libusb-1.0.dll)
#
# find_library(LIBUSB_LIBRARY NAMES libusb-1.0 PATHS ${LIBUSB_ROOT_DIR}/lib/ DOC "LIBUSB library" )
# #    find_library(LIBUSB_LIBRARY #        NAMES libusb-1.0 #        PATHS ${LIBUSB_ROOT_DIR}/dll/
# #        DOC "LIBUSB library" # ) message(STATUS "library : " ${LIBUSB_LIBRARY}) if
# (${LIBUSB_LIBRARY} STREQUAL "LIBUSB_LIBRARY-NOTFOUND") message(FATAL_ERROR "LIBUSB_LIBRARY not
# found") endif() add_library(libusb INTERFACE IMPORTED) # set(LIBUSB_INCLUDE_DIR
# ${LIBUSB_ROOT_DIR}/Inc)
#
# else() #    file(GLOB LIBUSB_SRC ${LIBUSB_ROOT_DIR}/src/*.cpp) # list(REMOVE_ITEM LIBUSB_SRC
# ${LIBUSB_ROOT_DIR}/src/PiTerm.cpp) #    file(GLOB LIBUSB_HEADERS ${LIBUSB_ROOT_DIR}/src/*.h) #
# list(REMOVE_ITEM LIBUSB_HEADERS ${LIBUSB_ROOT_DIR}/src/PiTerm.h)
#
# #    add_library(libusb-1.0 STATIC ${LIBUSB_SRC} ${LIBUSB_HEADERS}) # set(LIBUSB_LIBRARY
# libusb-1.0) #    set(LIBUSB_INCLUDE_DIR ${LIBUSB_ROOT_DIR}/src/) endif()
#
# include(FindPackageHandleStandardArgs) find_package_handle_standard_args(libusb REQUIRED_VARS
# LIBUSB_LIBRARY # LIBUSB_INCLUDE_DIR )
#
# mark_as_advanced(LIBUSB_FOUND LIBUSB_LIBRARY LIBUSB_INCLUDE_DIR )
#
# if (LIBUSB_FOUND) set_target_properties(libusb PROPERTIES INTERFACE_LINK_LIBRARIES
# ${LIBUSB_LIBRARY} # INTERFACE_INCLUDE_DIRECTORIES ${LIBUSB_INCLUDE_DIR} ) else()
# message(FATAL_ERROR "Could not find LIBUSB.") endif()
#

# * Find libusb for portable USB support This module will find libusb as published by
#   http://libusb.sf.net and http://libusb-win32.sf.net
#
# It will use PkgConfig if present and supported, else search it on its own. If the LibUSB_ROOT_DIR
# environment variable is defined, it will be used as base path. The following standard variables
# get defined: LibUSB_FOUND:        true if LibUSB was found LibUSB_INCLUDE_DIRS: the directory that
# contains the include file LibUSB_LIBRARIES:    the library

include(CheckLibraryExists)
include(CheckIncludeFile)

find_package(PkgConfig)
if(PKG_CONFIG_FOUND)
    pkg_check_modules(PKGCONFIG_LIBUSB libusb)
endif(PKG_CONFIG_FOUND)

if(PKGCONFIG_LIBUSB_FOUND)
    set(LibUSB_FOUND ${PKGCONFIG_LIBUSB_FOUND})
    set(LibUSB_INCLUDE_DIRS ${PKGCONFIG_LIBUSB_INCLUDE_DIRS})
    foreach(i ${PKGCONFIG_LIBUSB_LIBRARIES})
        find_library(
            ${i}_LIBRARY
            NAMES ${i}
            PATHS ${PKGCONFIG_LIBUSB_LIBRARY_DIRS})
        if(${i}_LIBRARY)
            list(APPEND LibUSB_LIBRARIES ${${i}_LIBRARY})
        endif(${i}_LIBRARY)
        mark_as_advanced(${i}_LIBRARY)
    endforeach(i)

else(PKGCONFIG_LIBUSB_FOUND)
    find_path(
        LibUSB_INCLUDE_DIRS
        NAMES usb.h
        PATHS $ENV{ProgramFiles}/LibUSB-Win32 $ENV{LibUSB_ROOT_DIR}
        PATH_SUFFIXES include)
    mark_as_advanced(LibUSB_INCLUDE_DIRS)
    # message ( STATUS "LibUSB include dir: ${LibUSB_INCLUDE_DIRS}" )

    if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
        # LibUSB-Win32 binary distribution contains several libs. Use the lib that got compiled with
        # the same compiler.
        if(MSVC)
            if(WIN32)
                set(LibUSB_LIBRARY_PATH_SUFFIX lib/msvc)
            else(WIN32)
                set(LibUSB_LIBRARY_PATH_SUFFIX lib/msvc_x64)
            endif(WIN32)
        elseif(BORLAND)
            set(LibUSB_LIBRARY_PATH_SUFFIX lib/bcc)
        elseif(CMAKE_COMPILER_IS_GNUCC)
            set(LibUSB_LIBRARY_PATH_SUFFIX lib/gcc)
        endif(MSVC)
    endif(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")

    find_library(
        usb_LIBRARY
        NAMES libusb usb
        PATHS $ENV{ProgramFiles}/LibUSB-Win32 $ENV{LibUSB_ROOT_DIR}
        PATH_SUFFIXES ${LibUSB_LIBRARY_PATH_SUFFIX})
    mark_as_advanced(usb_LIBRARY)
    if(usb_LIBRARY)
        set(LibUSB_LIBRARIES ${usb_LIBRARY})
    endif(usb_LIBRARY)

    if(LibUSB_INCLUDE_DIRS AND LibUSB_LIBRARIES)
        set(LibUSB_FOUND true)
    endif(LibUSB_INCLUDE_DIRS AND LibUSB_LIBRARIES)
endif(PKGCONFIG_LIBUSB_FOUND)

if(LibUSB_FOUND)
    set(CMAKE_REQUIRED_INCLUDES "${LibUSB_INCLUDE_DIRS}")
    check_include_file(usb.h LibUSB_FOUND)
    # message ( STATUS "LibUSB: usb.h is usable: ${LibUSB_FOUND}" )
endif(LibUSB_FOUND)
if(LibUSB_FOUND)
    check_library_exists("${LibUSB_LIBRARIES}" usb_open "" LibUSB_FOUND)
    # message ( STATUS "LibUSB: library is usable: ${LibUSB_FOUND}" )
endif(LibUSB_FOUND)

if(NOT LibUSB_FOUND)
    if(NOT LibUSB_FIND_QUIETLY)
        message(STATUS "LibUSB not found, try setting LibUSB_ROOT_DIR environment variable.")
    endif(NOT LibUSB_FIND_QUIETLY)
    if(LibUSB_FIND_REQUIRED)
        message(FATAL_ERROR "")
    endif(LibUSB_FIND_REQUIRED)
endif(NOT LibUSB_FOUND)
# message ( STATUS "LibUSB: ${LibUSB_FOUND}" )

message(STATUS "LibUSB name : " ${usb_LIBRARY})
