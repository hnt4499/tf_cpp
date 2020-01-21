cmake_minimum_required(VERSION 3.7)
project(tf_cpp_py)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp Model.cpp Model.h)
add_executable(main ${SOURCE_FILES})

# OpenCV libs
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries(main ${OpenCV_LIBS})
message(${OpenCV_INCLUDE_DIRS})
message(${OpenCV_LIBS})