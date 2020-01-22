#include "easycppogl_src/gl_viewer.h"
#include "easycppogl_src/shader_program.h"

#include <iostream>
#include <fstream>
#ifdef OSX
    #include <OpenGL/OpenGL.h>
#else
    #include <GL/gl.h>
#endif

using namespace EZCOGL;

#define BASE_FOLDER "../"

class Viewer : public GLViewer {
public:
    Viewer();
    void init_ogl() override;
    void draw_ogl() override;
    void interface_ogl() override;

private:
    std::shared_ptr<ShaderProgram> cubicBezierShaderProgram;
    std::shared_ptr<ShaderProgram> pointsShaderProgram;

    GLuint vao;

private:
    int outerTesselationLevel0;
    int outerTesselationLevel1;
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
        vao(0),
        outerTesselationLevel0(1),
        outerTesselationLevel1(1),
        color{1., 0., 0., 1.},
        pointsSize(10) {
}

void Viewer::init_ogl() {
    cubicBezierShaderProgram = ShaderProgram::create({
        {GL_VERTEX_SHADER, readFile("resources/shaders/basic_vert.glsl")},
        {GL_TESS_CONTROL_SHADER, readFile("resources/shaders/cubicBezier_tessCont.glsl")},
        {GL_TESS_EVALUATION_SHADER, readFile("resources/shaders/cubicBezier_tessEval.glsl")},
        {GL_FRAGMENT_SHADER, readFile("resources/shaders/basic_frag.glsl")}
    }, "");

    pointsShaderProgram = ShaderProgram::create({
        {GL_VERTEX_SHADER, readFile("resources/shaders/basic_vert.glsl")},
        {GL_FRAGMENT_SHADER, readFile("resources/shaders/basic_frag.glsl")}
    }, "");

    float vertices[] = {
            -0.5, -0.5, +0.0,
            -0.3, +0.25, +0.0,
            +0.5, +0.5, +0.0,
            +0.5, -0.5, +0.0
    };


    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);


    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);


    glClearColor(0., 0., 0., 1.);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Viewer::draw_ogl() {
    glClear(GL_COLOR_BUFFER_BIT);

    cubicBezierShaderProgram->bind();

    glUniform1f(
            glGetUniformLocation(cubicBezierShaderProgram->id(), "uOuterLevel0"),
            outerTesselationLevel0
    );
    glUniform1f(
            glGetUniformLocation(cubicBezierShaderProgram->id(), "uOuterLevel1"),
            outerTesselationLevel1
    );

    glUniform4fv(
            glGetUniformLocation(cubicBezierShaderProgram->id(), "uColor"),
            1,
            color
    );

    glBindVertexArray(vao);

    glPatchParameteri(GL_PATCH_VERTICES, 4);
    glDrawArrays(GL_PATCHES, 0, 4);
    glBindVertexArray(0);

    cubicBezierShaderProgram->unbind();


    pointsShaderProgram->bind();
    glBindVertexArray(vao);

    glUniform4f(
            glGetUniformLocation(pointsShaderProgram->id(), "uColor"),
            0., 1., 0., .3
    );
    glDrawArrays(GL_LINE_STRIP, 0, 4);

    glUniform4f(
            glGetUniformLocation(pointsShaderProgram->id(), "uColor"),
            0., 1., 0., 1.
    );
    glPointSize(pointsSize);
    glDrawArrays(GL_POINTS, 0, 4);

    glBindVertexArray(0);
    pointsShaderProgram->unbind();

}

void Viewer::interface_ogl() {
    bool ui_tesselation_level_show = true;
    ImGui::Begin("Parameters", &ui_tesselation_level_show);

    if (ImGui::TreeNode("Rendering")) {
        ImGui::ColorEdit4("Color", color);
        ImGui::DragInt("CP Size", &pointsSize, 1, 0, 40);

        ImGui::TreePop();
    }

    if (ImGui::TreeNode("Tesselation Levels")) {
        ImGui::SliderInt("Outer 0", &outerTesselationLevel0, 0, 50);
        ImGui::SliderInt("Outer 1", &outerTesselationLevel1, 0, 50);

        ImGui::TreePop();
    }

    ImGui::End();
}

int main() {
    Viewer viewer;
    viewer.launch3d();

    return 0;
}
