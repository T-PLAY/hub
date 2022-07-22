
#set(PDI_SDK_DIR "")
#find_path(PDI_SDK_DIR
#    Inc
#    PATHS "C:/Polhemus/PDI"
#    DOC "PDI include path"
#    #PATH_SUFFIXES Inc
#    )
#    message(STATUS "hello ${PDI_SDK_DIR}")


set(PDI_SDK "<UNSET>" CACHE PATH "Path of PDI SDK dir on your system")
if (WIN32 AND EXISTS "C:/Polhemus/PDI/PDI_140")
    set(PDI_SDK "C:/Polhemus/PDI/PDI_140")
endif()
if (PDI_SDK STREQUAL "<UNSET>")
    message(FATAL_ERROR "You must set PDI_SDK path of PDI_140 (WIN) or piterm-1.0.4 (LINUX) folder in your system see : https://ftp.polhemus1.com/pub/Trackers/Patriot/Software/ https://ftp.polhemus1.com/pub/Software/PDI_4.4.1_Setup-SWD-PS08-08.exe or https://ftp.polhemus1.com/pub/Software/Linux/")
endif()

if (WIN32)
    file(GLOB PDI_DLL ${PDI_SDK}/Lib/x64/P*.dll)
#    file(GLOB PDI_DLL ${PDI_SDK}/Lib/x64/PDI.dll)

    find_library(PDI_LIBRARY
        NAMES pdi
        PATHS ${PDI_SDK}/Lib/x64
        DOC "PDI library"
        )
    add_library(pdi INTERFACE IMPORTED)
    set(PDI_INCLUDE_DIR ${PDI_SDK}/Inc)

else()
    file(GLOB PDI_SRC ${PDI_SDK}/src/*.cpp)
    list(REMOVE_ITEM PDI_SRC ${PDI_SDK}/src/PiTerm.cpp)
    file(GLOB PDI_HEADERS ${PDI_SDK}/src/*.h)
    list(REMOVE_ITEM PDI_HEADERS ${PDI_SDK}/src/PiTerm.h)

    add_library(pdi STATIC ${PDI_SRC} ${PDI_HEADERS})
    set(PDI_LIBRARY pdi)
    set(PDI_INCLUDE_DIR ${PDI_SDK}/src/)
endif()


include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(pdi
    REQUIRED_VARS
    PDI_LIBRARY
    PDI_INCLUDE_DIR
    )

mark_as_advanced(PDI_FOUND
    PDI_LIBRARY
    PDI_INCLUDE_DIR
    )

if (PDI_FOUND)
        set_target_properties(pdi PROPERTIES
            INTERFACE_LINK_LIBRARIES ${PDI_LIBRARY}
            INTERFACE_INCLUDE_DIRECTORIES ${PDI_INCLUDE_DIR}
        )
else()
    message(FATAL_ERROR "Could not find PDI.")
endif()

