#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "hub" for configuration "Debug"
set_property(TARGET hub APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(hub PROPERTIES
  IMPORTED_LOCATION_DEBUG "/ARM/hubd.dll"
  )

list(APPEND _cmake_import_check_targets hub )
list(APPEND _cmake_import_check_files_for_hub "/ARM/hubd.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
