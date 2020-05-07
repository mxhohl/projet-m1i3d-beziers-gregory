cmake_minimum_required(VERSION 3.0)
project(bezier)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(easycppogl)

include_directories(./common)

set(COMMON_SRC
        ./common/BezierCurve.hpp
        ./common/BezierCurve.cpp
        ./common/BezierRenderer.hpp
        ./common/BezierRenderer.cpp
        ./common/RenderableBezier.hpp
        ./common/utils.hpp
        ./common/BezierRectSurface.cpp
        ./common/BezierRectSurface.hpp common/GregoryRectSurface.cpp common/GregoryRectSurface.hpp)

add_executable(curves
        ./curves/main.cpp
        ./curves/Viewer.cpp
        ./curves/Viewer.hpp
        ${COMMON_SRC})
target_link_libraries(curves easycppogl)
target_compile_definitions(curves PRIVATE
        "-DRESOURCES=${CMAKE_SOURCE_DIR}/resources")
target_compile_options(curves PRIVATE -Wall -Wextra)

add_executable(rect_surf
        ./rect_surface/main.cpp
        ./rect_surface/Viewer.cpp
        ./rect_surface/Viewer.hpp
        ${COMMON_SRC})
target_link_libraries(rect_surf easycppogl)
target_compile_definitions(rect_surf PRIVATE
        "-DRESOURCES=${CMAKE_SOURCE_DIR}/resources")
target_compile_options(rect_surf PRIVATE -Wall -Wextra)

add_executable(gregory
        ./gregory/main.cpp
        ${COMMON_SRC} gregory/Viewer.cpp gregory/Viewer.hpp)
target_link_libraries(gregory easycppogl)
target_compile_definitions(gregory PRIVATE
        "-DRESOURCES=${CMAKE_SOURCE_DIR}/resources")
target_compile_options(gregory PRIVATE -Wall -Wextra)
