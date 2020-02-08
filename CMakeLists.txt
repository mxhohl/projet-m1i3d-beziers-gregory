cmake_minimum_required(VERSION 3.0)
project(bezier)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(easycppogl)

include_directories(./common)

add_subdirectory(curves)
add_subdirectory(rect_surface)
