cmake_minimum_required(VERSION 3.8)

project(DicomUtils)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)




find_package(DCMTK REQUIRED)



file(GLOB_RECURSE sources *.cpp)
file(GLOB_RECURSE headers *.h)


add_library(${PROJECT_NAME} ${sources} ${headers} # ${app_uis}
)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)
target_include_directories(
    ${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
                           # PRIVATE ${RADIUM_INCLUDE_DIRS} ${CMAKE_CURRENT_BINARY_DIR} # Moc
)

target_link_libraries(
    ${PROJECT_NAME}
    PUBLIC # Radium::Core Radium::Engine Radium::Gui ${Qt_LIBRARIES} ${DCMTK_LIBRARIES} PRIVATE
           DCMTK::dcmdata DCMTK::dcmimage)

