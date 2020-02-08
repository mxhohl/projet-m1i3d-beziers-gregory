add_executable(rect_surf main.cpp Viewer.cpp Viewer.hpp)
target_link_libraries(rect_surf easycppogl)
target_compile_definitions(rect_surf PRIVATE
        "-DRESOURCES=${CMAKE_SOURCE_DIR}/resources")
