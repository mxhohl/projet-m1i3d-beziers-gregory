add_executable(curves main.cpp)
target_link_libraries(curves easycppogl)
target_compile_definitions(curves PRIVATE
        "-DRESOURCES=${CMAKE_SOURCE_DIR}/resources")
