
set(PDI_SDK "<UNSET>" CACHE PATH "Path of PDI SDK dir on your system")
if (PDI_SDK STREQUAL "<UNSET>")
    message(WARNING "You must set PDI_SDK path of PDI_140 folder in your system")
    return()
endif()


find_path(PDI_INCLUDE_DIR
    NAMES PDI.h
    PATHS ${PDI_SDK}/Inc
    DOC "PDI include path"
    )

if (WIN32)
    set(PDI_LIBDIR "Lib/x64/")
    file(GLOB PDI_DLL ${PDI_SDK}/Lib/x64/P*.dll)
#    file(GLOB PDI_DLL ${PDI_SDK}/Lib/x64/PDI.dll)
else()
    set(PDI_LIBDIR "bin")
endif()

find_library(PDI_LIBRARY
    NAMES pdi
    PATHS ${PDI_SDK}/${PDI_LIBDIR}
    DOC "PDI library"
    )

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
        add_library(pdi INTERFACE IMPORTED)
        set_target_properties(pdi PROPERTIES
            INTERFACE_LINK_LIBRARIES ${PDI_LIBRARY}
            INTERFACE_INCLUDE_DIRECTORIES ${PDI_INCLUDE_DIR}
        )
else()
    message(FATAL_ERROR "Could not find PDI.")
endif()

