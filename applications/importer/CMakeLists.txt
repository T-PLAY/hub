cmake_minimum_required(VERSION 3.8)

if (HUB_USE_DCMTK)
        add_subdirectory(dicomImporter)
        add_subdirectory(libDicomUtils)
endif()
