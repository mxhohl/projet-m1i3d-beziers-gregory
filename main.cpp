#include "easycppogl_src/gl_viewer.h"
#include "easycppogl_src/shader_program.h"

#include <iostream>
#include <fstream>
#include <string>
#include <random>

using namespace EZCOGL;

#define MACRO_STR(s) #s
#define MACRO_XSTR(s) MACRO_STR(s)
#define RESOURCE_PATH std::string(MACRO_XSTR(RESOURCES)) + '/'

enum class Mode {
    BezierCurve = 0,
    BezierSurface = 1
};

std::string to_string(Mode mode) {
    switch (mode) {
        case Mode::BezierCurve:
            return "Bezier Curve";
        case Mode::BezierSurface:
            return "Bezier Surface";
        default:
            return "Unknown";
    }
}

enum class DrawMode {
    Point = 0,
    Line = 1,
    Fill = 2
};

std::string to_string(DrawMode mode) {
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

GLenum gl_draw_mode(DrawMode mode) {
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

class Viewer : public GLViewer {
public:
    Viewer();
    void init_ogl() override;
    void draw_ogl() override;
    void interface_ogl() override;

private:
    void init_bezierCurves_vao();
    void init_bezierSurfaces_vao();

private:
    std::shared_ptr<ShaderProgram> bezierCurveShaderProgram;
    std::shared_ptr<ShaderProgram> bezierSurfaceShaderProgram;
    std::shared_ptr<ShaderProgram> pointsShaderProgram;
    std::shared_ptr<ShaderProgram> transformablePointsShaderProgram;

    std::shared_ptr<VAO> vaoBezierCurves;
    std::shared_ptr<VAO> vaoBezierSurfaces;
    GLuint surfaceDimU;
    GLuint surfaceDimV;

private:
    Mode mode;
    DrawMode drawMode;

    int outerTesselationLevel0;
    int outerTesselationLevel1;

    int innerTesselationLevel0;
    int innerTesselationLevel1;
    int outerTesselationLevel2;
    int outerTesselationLevel3;

    float color[4];
    int pointsSize;
};

std::string readFile(const std::string& path) {
    const std::string full_path = RESOURCE_PATH + path;
    std::ifstream file{full_path, std::ifstream::in};
    if (!file.good()) {
        std::cerr << "Unable to open file '" << full_path << "'" << std::endl;
        return "";
    }
    return std::string(std::istreambuf_iterator<char>(file),
                       std::istreambuf_iterator<char>());
}

Viewer::Viewer() :
        vaoBezierCurves(nullptr),
        vaoBezierSurfaces(nullptr),
        surfaceDimU(0),
        surfaceDimV(0),
        mode(Mode::BezierCurve),
        drawMode(DrawMode::Fill),
        outerTesselationLevel0(1),
        outerTesselationLevel1(1),
        innerTesselationLevel0(1),
        innerTesselationLevel1(1),
        outerTesselationLevel2(1),
        outerTesselationLevel3(1),
        color{1., 0., 0., 1.},
        pointsSize(10) {
}

void Viewer::init_bezierCurves_vao() {
    std::vector<GLVec3> vertices = {
            GLVec3{-0.5, -0.5, +0.0},
            GLVec3{-0.3, +0.25, +0.0},
            GLVec3{+0.5, +0.5, +0.0},
            GLVec3{+0.0, -0.75, +0.0},
            GLVec3{+0.5, -0.5, +0.0},
    };

    auto vbo = VBO::create(vertices);

    vaoBezierCurves = VAO::create({
        {0, vbo}
    });
}

void Viewer::init_bezierSurfaces_vao() {
    constexpr size_t dimU = 4;
    constexpr size_t dimV = 4;

    std::vector<GLVec3> vertices(dimU * dimV);

    std::default_random_engine generator(
            std::chrono::system_clock::now().time_since_epoch().count()
    );
    std::uniform_real_distribution<float> distribution(0.f, 2.f);
    auto rand = std::bind(distribution, generator);

    constexpr float offset = 1.f;
    constexpr float uHalfSize = (offset * dimU) / 2.f;
    constexpr float vHalfSize = (offset * dimV) / 2.f;

    for (size_t u = 0; u < dimU; ++u) {
        for (size_t v = 0; v < dimV; ++v) {
            vertices.emplace_back(
                    (u * offset) - uHalfSize,
                    (v * offset) - vHalfSize,
                    rand()
            );
        }
    }

    auto vbo = VBO::create(vertices);
    vaoBezierSurfaces = VAO::create({
        {0, vbo}
    });
    surfaceDimU = dimU;
    surfaceDimV = dimV;
}

void Viewer::init_ogl() {
    bezierCurveShaderProgram = ShaderProgram::create({
        {
            GL_VERTEX_SHADER,
            readFile("shaders/basic_vert.glsl")
        }, {
            GL_TESS_CONTROL_SHADER,
            readFile("shaders/curveBezier_tessCont.glsl")
        }, {
            GL_TESS_EVALUATION_SHADER,
            readFile("shaders/curveBezier_tessEval.glsl")
        }, {
            GL_FRAGMENT_SHADER,
            readFile("shaders/basic_frag.glsl")
        }
    }, "");

    bezierSurfaceShaderProgram = ShaderProgram::create({
        {
            GL_VERTEX_SHADER,
            readFile("shaders/basicTransformable_vert.glsl")
        }, {
            GL_TESS_CONTROL_SHADER,
            readFile("shaders/surfaceBezier_tessCont.glsl")
        }, {
            GL_TESS_EVALUATION_SHADER,
            readFile("shaders/surfaceBezier_tessEval.glsl")
        }, {
            GL_FRAGMENT_SHADER,
            readFile("shaders/basic_frag.glsl")
        }
    }, "");

    pointsShaderProgram = ShaderProgram::create({
        {
            GL_VERTEX_SHADER,
            readFile("shaders/basic_vert.glsl")
        }, {
            GL_FRAGMENT_SHADER,
            readFile("shaders/basic_frag.glsl")
        }
    }, "");

    transformablePointsShaderProgram = ShaderProgram::create({
        {
            GL_VERTEX_SHADER,
            readFile("shaders/basicTransformable_vert.glsl")
        }, {
            GL_FRAGMENT_SHADER,
            readFile("shaders/basic_frag.glsl")
        }
    }, "");

    init_bezierCurves_vao();
    init_bezierSurfaces_vao();

    set_scene_center(GLVec3(0, 0, 0));
    set_scene_radius(3.0);

    glClearColor(0., 0., 0., 1.);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Viewer::draw_ogl() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPointSize(pointsSize);

    glPolygonMode(GL_FRONT_AND_BACK, gl_draw_mode(drawMode));

    if (mode == Mode::BezierCurve) {
        const auto& cpCount = vaoBezierCurves->length();

        bezierCurveShaderProgram->bind();

        set_uniform_value("uColor", GLVec4(color));
        set_uniform_value("uOuterLevel0", static_cast<GLfloat>(outerTesselationLevel0));
        set_uniform_value("uOuterLevel1", static_cast<GLfloat>(outerTesselationLevel1));
        set_uniform_value("uCPCount", static_cast<GLuint>(cpCount));

        vaoBezierCurves->bind();
        glPatchParameteri(GL_PATCH_VERTICES, cpCount);
        glDrawArrays(GL_PATCHES, 0, cpCount);
        vaoBezierCurves->unbind();

        bezierCurveShaderProgram->unbind();


        pointsShaderProgram->bind();
        vaoBezierCurves->bind();

        set_uniform_value("uColor", GLVec4({0., 1., 0., .3}));
        glDrawArrays(GL_LINE_STRIP, 0, cpCount);

        set_uniform_value("uColor", GLVec4({0., 1., 0., 1.}));
        glDrawArrays(GL_POINTS, 0, cpCount);

        vaoBezierCurves->unbind();
        pointsShaderProgram->unbind();

    } else if (mode == Mode::BezierSurface) {
        const auto& cpCount = vaoBezierSurfaces->length();

        const auto& projMat = this->get_projection_matrix();
        const auto& mvMat = this->get_modelview_matrix();

        bezierSurfaceShaderProgram->bind();

        set_uniform_value("projMatrix", projMat);
        set_uniform_value("mvMatrix", mvMat);
        set_uniform_value("uColor", GLVec4(color));

        set_uniform_value("uInnerLevel0", static_cast<GLfloat>(innerTesselationLevel0));
        set_uniform_value("uInnerLevel1", static_cast<GLfloat>(innerTesselationLevel1));
        set_uniform_value("uOuterLevel0", static_cast<GLfloat>(outerTesselationLevel0));
        set_uniform_value("uOuterLevel1", static_cast<GLfloat>(outerTesselationLevel1));
        set_uniform_value("uOuterLevel2", static_cast<GLfloat>(outerTesselationLevel2));
        set_uniform_value("uOuterLevel3", static_cast<GLfloat>(outerTesselationLevel3));

        set_uniform_value("uCPUCount", surfaceDimU);
        set_uniform_value("uCPVCount", surfaceDimV);

        vaoBezierSurfaces->bind();
        glPatchParameteri(GL_PATCH_VERTICES, cpCount);
        glDrawArrays(GL_PATCHES, 0, cpCount);
        vaoBezierSurfaces->unbind();

        bezierSurfaceShaderProgram->unbind();


        transformablePointsShaderProgram->bind();

        set_uniform_value("projMatrix", projMat);
        set_uniform_value("mvMatrix", mvMat);
        set_uniform_value("uColor", GLVec4({0., 1., 0., 1.}));

        vaoBezierSurfaces->bind();
        glDrawArrays(GL_POINTS, 0, cpCount);
        vaoBezierSurfaces->unbind();

        transformablePointsShaderProgram->unbind();
    }
}

void Viewer::interface_ogl() {
    bool ui_tesselation_level_show = true;
    ImGui::Begin("Parameters", &ui_tesselation_level_show);

    ImGui::SliderInt(
            ("Mode - " + to_string(mode)).c_str(),
            reinterpret_cast<int*>(&mode),
            0, 1
    );

    if (ImGui::TreeNode("Rendering")) {
        ImGui::SliderInt(
                ("Draw Mode - " + to_string(drawMode)).c_str(),
                reinterpret_cast<int*>(&drawMode),
                0, 2
        );
        ImGui::ColorEdit4("Color", color);
        ImGui::SliderInt("CP Size", &pointsSize, 0, 40);

        ImGui::TreePop();
    }

    if (ImGui::TreeNode(("Parameters - " + to_string(mode)).c_str())) {
        if (mode == Mode::BezierCurve) {
            ImGui::SliderInt(
                    "Outer Tess Level 0",
                    &outerTesselationLevel0,
                    0, 50
            );
            ImGui::SliderInt(
                    "Outer Tess Level 1",
                    &outerTesselationLevel1,
                    0, 50
            );
        } else if (mode == Mode::BezierSurface) {
            ImGui::SliderInt(
                    "Inner Tess Level 0",
                    &innerTesselationLevel0,
                    0, 50
            );
            ImGui::SliderInt(
                    "Inner Tess Level 1",
                    &innerTesselationLevel1,
                    0, 50
            );

            ImGui::SliderInt(
                    "Outer Tess Level 0",
                    &outerTesselationLevel0,
                    0, 50
            );
            ImGui::SliderInt(
                    "Outer Tess Level 1",
                    &outerTesselationLevel1,
                    0, 50
            );
            ImGui::SliderInt(
                    "Outer Tess Level 2",
                    &outerTesselationLevel2,
                    0, 50
            );
            ImGui::SliderInt(
                    "Outer Tess Level 3",
                    &outerTesselationLevel3,
                    0, 50
            );
        }

        ImGui::TreePop();
    }

    ImGui::End();
}

int main() {
    Viewer viewer;
    viewer.launch3d();

    return 0;
}
