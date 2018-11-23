cmake_minimum_required(VERSION 3.10)
project(dubins)

cmake_minimum_required(VERSION 3.5)

project(opencv)


find_package(PkgConfig REQUIRED)
pkg_search_module( TESSERACT REQUIRED tesseract )
find_package(OpenCV REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
file(GLOB source_files
        "*.h"
        "*.cpp"
        "Headers/*.hpp"
        "Source/*.cpp"
        "config/*"
        )

set(SOURCE_FILES main.cpp)
pkg_search_module( TESSERACT REQUIRED tesseract )
add_executable(dubins ${source_files} Source/Position.cpp Source/Car.cpp Source/DubinPath.cpp Source/DubinArc.cpp Source/PossbilePath.cpp)



include_directories(${OpenCV_INCLUDE_DIRS})

target_link_libraries(dubins ${OpenCV_LIBS} )
target_link_libraries(dubins ${TESSERACT_LIBRARIES} )