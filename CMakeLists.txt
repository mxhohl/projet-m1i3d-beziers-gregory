cmake_minimum_required(VERSION 3.15)
project(bezier)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(easycppogl)

add_executable(bezier main.cpp)
target_link_libraries(bezier easycppogl)
