cmake_minimum_required(VERSION 3.11)
project(videoReorder VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

message(STATUS "C++ standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "Compiler version: ${CMAKE_CXX_COMPILER_VERSION}")



## get opencv package
find_package(OpenCV REQUIRED )
find_package(OpenMP REQUIRED)

include_directories( ${OpenCV_INCLUDE_DIRS} )


# to check cpp version
add_executable(cpp_version cppVersion.cpp)
target_link_libraries(cpp_version OpenMP::OpenMP_CXX ${OpenCV_LIBS} )

add_executable( videoToImage src/videoToImage.cpp)
target_link_libraries(videoToImage  ${OpenCV_LIBS} )


# add_executable(videoReorder src/main.cpp include/ssim.hpp)
add_executable(videoReorder src/main.cpp src/ssim.cpp src/corruptImages.cpp src/ordering.cpp)
target_include_directories(videoReorder PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(videoReorder OpenMP::OpenMP_CXX ${OpenCV_LIBS})