#include "easycppogl_src/gl_viewer.h"
#include "easycppogl_src/shader_program.h"

#include <iostream>
#include <fstream>
#include <GL/gl.h>

using namespace EZCOGL;

#define BASE_FOLDER "../"

class Viewer : public GLViewer {
public:
    Viewer();
    void init_ogl() override;
    void draw_ogl() override;
    void interface_ogl() override;

private:
    std::shared_ptr<ShaderProgram> shaderProgram;

    GLuint vbo;
    GLuint vao;

private:
    int innerTesselationLevel;
    int outerTesselationLevel0;
    int outerTesselationLevel1;
    int outerTesselationLevel2;
    float color[4];
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
        vbo(0), vao(0),
        innerTesselationLevel(1),
        outerTesselationLevel0(1),
        outerTesselationLevel1(1),
        outerTesselationLevel2(1),
        color{1., 0., 0., 1.} {
}

void Viewer::init_ogl() {
    shaderProgram = ShaderProgram::create({
        {GL_VERTEX_SHADER, readFile("resources/shaders/test_vert.glsl")},
        {GL_TESS_CONTROL_SHADER, readFile("resources/shaders/test_tessCont.glsl")},
        {GL_TESS_EVALUATION_SHADER, readFile("resources/shaders/test_tessEval.glsl")},
        {GL_FRAGMENT_SHADER, readFile("resources/shaders/test_frag.glsl")}
    }, "");

    float vertices[] = {
            +0.0, +0.5, +0.0,
            +0.5, -0.5, +0.0,
            -0.5, -0.5, +0.0
    };

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


    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glClearColor(0., 0., 0., 1.);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Viewer::draw_ogl() {
    shaderProgram->bind();
    glClear(GL_COLOR_BUFFER_BIT);

    glUniform1f(
            glGetUniformLocation(shaderProgram->id(), "uInnerLevel"),
            innerTesselationLevel
    );
    glUniform1f(
            glGetUniformLocation(shaderProgram->id(), "uOuterLevel0"),
            outerTesselationLevel0
    );
    glUniform1f(
            glGetUniformLocation(shaderProgram->id(), "uOuterLevel1"),
            outerTesselationLevel1
    );
    glUniform1f(
            glGetUniformLocation(shaderProgram->id(), "uOuterLevel2"),
            outerTesselationLevel2
    );

    glUniform4fv(
            glGetUniformLocation(shaderProgram->id(), "uColor"),
            1,
            color
    );

    glBindVertexArray(vao);

    glPatchParameteri(GL_PATCH_VERTICES, 3);
    glDrawArrays(GL_PATCHES, 0, 3);
    glBindVertexArray(0);

    shaderProgram->unbind();
}

void Viewer::interface_ogl() {
    bool ui_tesselation_level_show = true;
    ImGui::Begin("Parameters", &ui_tesselation_level_show);

    if (ImGui::TreeNode("Rendering")) {
        ImGui::ColorEdit4("Color", color);

        ImGui::TreePop();
    }

    if (ImGui::TreeNode("Tesselation Levels")) {
        ImGui::SliderInt("Inner", &innerTesselationLevel, 1, 20);
        ImGui::SliderInt("Outer 0", &outerTesselationLevel0, 1, 20);
        ImGui::SliderInt("Outer 1", &outerTesselationLevel1, 1, 20);
        ImGui::SliderInt("Outer 2", &outerTesselationLevel2, 1, 20);

        ImGui::TreePop();
    }

    ImGui::End();
}

int main() {
    Viewer viewer;
    viewer.launch3d();

    return 0;
}
