
option(HUB_API_OCTAVE "Enable octave api" OFF)
if (HUB_API_OCTAVE)
    add_subdirectory(octave)
endif()

option(HUB_API_MATLAB "Enable matlab api" OFF)
if (HUB_API_MATLAB)

    set(Matlab_ROOT_DIR
        "<UNSET>"
            CACHE PATH "Path of Matlab dir on your system")

    if(Matlab_ROOT_DIR STREQUAL "<UNSET>")
            message(FATAL_ERROR "You must set Matlab_ROOT_DIR path of your Matlab folder in your system containing <Matlab_ROOT_DIR>/bin/{matlab,mex} executable files")
    else()
        add_subdirectory(matlab)
    endif()

endif()
