#include "easycppogl_src/gl_viewer.h"
#include "easycppogl_src/shader_program.h"

#include <iostream>
#include <fstream>
#include <string>
#ifdef OSX
    #include <OpenGL/OpenGL.h>
#else
    #include <GL/gl.h>
#endif

using namespace EZCOGL;

#define BASE_FOLDER "../"

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

    GLuint vaoBezierCurves;
    size_t bezierCurvesCPCount;
    GLuint vaoBezierSurfaces;
    size_t bezierSurfacesCPCount;

private:
    Mode mode;

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
    std::ifstream file{BASE_FOLDER + path, std::ifstream::in};
    if (!file.good()) {
        std::cerr << "Unable to open file '" << path << "'" << std::endl;
        return "";
    }
    return std::string(std::istreambuf_iterator<char>(file),
                       std::istreambuf_iterator<char>());
}

Viewer::Viewer() :
        vaoBezierCurves(0),
        bezierCurvesCPCount(0),
        vaoBezierSurfaces(0),
        bezierSurfacesCPCount(0),
        mode(Mode::BezierCurve),
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
    float vertices[] = {
            -0.5, -0.5, +0.0,
            -0.3, +0.25, +0.0,
            +0.5, +0.5, +0.0,
            +0.5, -0.5, +0.0
    };
    bezierCurvesCPCount = 4;


    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);


    glGenVertexArrays(1, &vaoBezierCurves);
    glBindVertexArray(vaoBezierCurves);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Viewer::init_bezierSurfaces_vao() {
    bezierSurfacesCPCount = 4 * 4;

    float vertices[3 * bezierSurfacesCPCount];

    srand(time(nullptr));
    const float offset = 1;
    const float hSize = (offset * 4.f) / 2.f;
    for (size_t x = 0; x < 4; ++x) {
        for (size_t y = 0; y < 4; ++y) {
            vertices[(y * 4 + x) * 3] = (x * offset) - hSize;
            vertices[(y * 4 + x) * 3 + 1] = (y * offset) - hSize;
            vertices[(y * 4 + x) * 3 + 2] = static_cast<float>(rand() % 2);
        }
    }

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);


    glGenVertexArrays(1, &vaoBezierSurfaces);
    glBindVertexArray(vaoBezierSurfaces);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Viewer::init_ogl() {
    bezierCurveShaderProgram = ShaderProgram::create({
        {GL_VERTEX_SHADER, readFile("resources/shaders/basic_vert.glsl")},
        {GL_TESS_CONTROL_SHADER, readFile("resources/shaders/curveBezier_tessCont.glsl")},
        {GL_TESS_EVALUATION_SHADER, readFile("resources/shaders/curveBezier_tessEval.glsl")},
        {GL_FRAGMENT_SHADER, readFile("resources/shaders/basic_frag.glsl")}
    }, "");

    bezierSurfaceShaderProgram = ShaderProgram::create({
        {GL_VERTEX_SHADER, readFile("resources/shaders/basicTransformable_vert.glsl")},
        {GL_TESS_CONTROL_SHADER, readFile("resources/shaders/surfaceBezier_tessCont.glsl")},
        {GL_TESS_EVALUATION_SHADER, readFile("resources/shaders/surfaceBezier_tessEval.glsl")},
        {GL_FRAGMENT_SHADER, readFile("resources/shaders/basic_frag.glsl")}
    }, "");

    pointsShaderProgram = ShaderProgram::create({
        {GL_VERTEX_SHADER, readFile("resources/shaders/basic_vert.glsl")},
        {GL_FRAGMENT_SHADER, readFile("resources/shaders/basic_frag.glsl")}
    }, "");

    transformablePointsShaderProgram = ShaderProgram::create({
        {GL_VERTEX_SHADER, readFile("resources/shaders/basicTransformable_vert.glsl")},
        {GL_FRAGMENT_SHADER, readFile("resources/shaders/basic_frag.glsl")}
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

    if (mode == Mode::BezierCurve) {
        bezierCurveShaderProgram->bind();

        glUniform1f(
                glGetUniformLocation(bezierCurveShaderProgram->id(), "uOuterLevel0"),
                outerTesselationLevel0
        );
        glUniform1f(
                glGetUniformLocation(bezierCurveShaderProgram->id(), "uOuterLevel1"),
                outerTesselationLevel1
        );

        glUniform4fv(
                glGetUniformLocation(bezierCurveShaderProgram->id(), "uColor"),
                1,
                color
        );

        glBindVertexArray(vaoBezierCurves);
        glPatchParameteri(GL_PATCH_VERTICES, bezierCurvesCPCount);
        glDrawArrays(GL_PATCHES, 0, bezierCurvesCPCount);
        glBindVertexArray(0);

        bezierCurveShaderProgram->unbind();


        pointsShaderProgram->bind();
        glBindVertexArray(vaoBezierCurves);

        glUniform4f(
                glGetUniformLocation(pointsShaderProgram->id(), "uColor"),
                0., 1., 0., .3
        );
        glDrawArrays(GL_LINE_STRIP, 0, bezierCurvesCPCount);

        glUniform4f(
                glGetUniformLocation(pointsShaderProgram->id(), "uColor"),
                0., 1., 0., 1.
        );
        glDrawArrays(GL_POINTS, 0, bezierCurvesCPCount);

        glBindVertexArray(0);
        pointsShaderProgram->unbind();
    } else if (mode == Mode::BezierSurface) {
        const auto& projMat = this->get_projection_matrix();
        const auto& mvMat = this->get_modelview_matrix();

        bezierSurfaceShaderProgram->bind();

        set_uniform_value("projMatrix", projMat);
        set_uniform_value("mvMatrix", mvMat);

        glUniform1f(
                glGetUniformLocation(bezierSurfaceShaderProgram->id(), "uInnerLevel0"),
                innerTesselationLevel0
        );
        glUniform1f(
                glGetUniformLocation(bezierSurfaceShaderProgram->id(), "uInnerLevel1"),
                innerTesselationLevel1
        );

        glUniform1f(
                glGetUniformLocation(bezierSurfaceShaderProgram->id(), "uOuterLevel0"),
                outerTesselationLevel0
        );
        glUniform1f(
                glGetUniformLocation(bezierSurfaceShaderProgram->id(), "uOuterLevel1"),
                outerTesselationLevel1
        );
        glUniform1f(
                glGetUniformLocation(bezierSurfaceShaderProgram->id(), "uOuterLevel2"),
                outerTesselationLevel2
        );
        glUniform1f(
                glGetUniformLocation(bezierSurfaceShaderProgram->id(), "uOuterLevel3"),
                outerTesselationLevel3
        );

        glUniform4fv(
                glGetUniformLocation(bezierSurfaceShaderProgram->id(), "uColor"),
                1,
                color
        );

        glBindVertexArray(vaoBezierSurfaces);
        glPatchParameteri(GL_PATCH_VERTICES, bezierSurfacesCPCount);
        glDrawArrays(GL_PATCHES, 0, bezierSurfacesCPCount);
        glBindVertexArray(0);

        bezierSurfaceShaderProgram->unbind();


        transformablePointsShaderProgram->bind();
        glBindVertexArray(vaoBezierSurfaces);

        set_uniform_value("projMatrix", projMat);
        set_uniform_value("mvMatrix", mvMat);

        glUniform4f(
                glGetUniformLocation(transformablePointsShaderProgram->id(), "uColor"),
                0., 1., 0., 1.
        );
        glDrawArrays(GL_POINTS, 0, bezierSurfacesCPCount);

        glBindVertexArray(0);
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
        ImGui::ColorEdit4("Color", color);
        ImGui::SliderInt("CP Size", &pointsSize, 0, 40);

        ImGui::TreePop();
    }

    if (ImGui::TreeNode(("Parameters - " + to_string(mode)).c_str())) {
        if (mode == Mode::BezierCurve) {
            ImGui::SliderInt("Outer Tess Level 0", &outerTesselationLevel0, 0, 50);
            ImGui::SliderInt("Outer Tess Level 1", &outerTesselationLevel1, 0, 50);
        } else if (mode == Mode::BezierSurface) {
            ImGui::SliderInt("Inner Tess Level 0", &innerTesselationLevel0, 0, 50);
            ImGui::SliderInt("Inner Tess Level 1", &innerTesselationLevel1, 0, 50);

            ImGui::SliderInt("Outer Tess Level 0", &outerTesselationLevel0, 0, 50);
            ImGui::SliderInt("Outer Tess Level 1", &outerTesselationLevel1, 0, 50);
            ImGui::SliderInt("Outer Tess Level 2", &outerTesselationLevel2, 0, 50);
            ImGui::SliderInt("Outer Tess Level 3", &outerTesselationLevel3, 0, 50);
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
