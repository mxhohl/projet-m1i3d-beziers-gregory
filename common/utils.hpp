#ifndef BEZIER_UTILS_HPP
#define BEZIER_UTILS_HPP

#include <fstream>

#define MACRO_STR(s) #s
#define MACRO_XSTR(s) MACRO_STR(s)
#define RESOURCE_PATH std::string(MACRO_XSTR(RESOURCES)) + '/'

inline std::string readFile(const std::string& path) {
    const std::string full_path = RESOURCE_PATH + path;
    std::ifstream file{full_path, std::ifstream::in};
    if (!file.good()) {
        std::cerr << "Unable to open file '" << full_path << "'" << std::endl;
        return "";
    }
    return std::string(std::istreambuf_iterator<char>(file),
                       std::istreambuf_iterator<char>());
}

enum class DrawMode {
    Point = 0,
    Line = 1,
    Fill = 2
};

inline std::string to_string(DrawMode mode) {
    switch (mode) {
        case DrawMode::Point:
            return "Point";
        case DrawMode::Line:
            return "Line";
        case DrawMode::Fill:
            return "Fill";
        default:
            return "Unknown";
    }
}

inline GLenum gl_draw_mode(DrawMode mode) {
    switch (mode) {
        case DrawMode::Point:
            return GL_POINT;
        case DrawMode::Line:
            return GL_LINE;
        case DrawMode::Fill:
            return GL_FILL;
        default:
            std::cerr << "Unknwn draw mode. Using GL_POINT." << std::endl;
            return GL_POINT;
    }
}

#endif //BEZIER_UTILS_HPP
