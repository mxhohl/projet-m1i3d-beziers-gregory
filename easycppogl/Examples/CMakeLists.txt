cmake_minimum_required(VERSION 3.0)
project(EasyCppOGL_Examples LANGUAGES CXX)

set (SYS_DATA_PATH $ENV{HOME} CACHE PATH "Path where to read image mesh ...")

add_definitions("-DDEF_DATA_PATH=${SYS_DATA_PATH}")

# Add your custom program
add_executable(tp_geometry_shader_billboard tp_geometry_shader_billboard.cpp)
target_link_libraries(tp_geometry_shader_billboard easycppogl)
