
option(HUB_API_OCTAVE "Enable octave api" OFF)
if (HUB_API_OCTAVE)
    add_subdirectory(octave)
endif()
