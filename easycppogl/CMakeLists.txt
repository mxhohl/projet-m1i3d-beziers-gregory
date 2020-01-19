cmake_minimum_required(VERSION 3.0)
project(EasyCppOGL)

set (CMAKE_CXX_STANDARD 11)

option(BUILD_EXAMPLES "Build some example apps." ON)


if(MSVC)
add_compile_options(/arch:SSE4)
else()
    add_compile_options(-msse4)
endif()

if(APPLE)
	link_directories("/usr/local/lib")
	message(WARNING MACOSX)
endif()


add_subdirectory(easycppogl_src)

if (${BUILD_EXAMPLES})
	add_subdirectory(Examples)
endif()
