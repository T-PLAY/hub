
option(HUB_API_OCTAVE "Enable octave api" OFF)
if (HUB_API_OCTAVE)
    add_subdirectory(octave)
endif()

option(HUB_API_MATLAB "Enable matlab api" OFF)
if (HUB_API_MATLAB)
    add_subdirectory(matlab)
endif()
