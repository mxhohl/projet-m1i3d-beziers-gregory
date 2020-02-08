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


class Viewer : public GLViewer {
public:
    Viewer();
    void init_ogl() override;
    void draw_ogl() override;
    void interface_ogl() override;

private:
    void init_vao();

private:
    std::shared_ptr<ShaderProgram> bezierCurveShaderProgram;
    std::shared_ptr<ShaderProgram> pointsShaderProgram;

    std::shared_ptr<VAO> vao;

private:
    int outerTesselationLevel1;

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
        vao(nullptr),
        outerTesselationLevel1(1),
        color{1., 0., 0., 1.},
        pointsSize(10) {
}

void Viewer::init_vao() {
    std::vector<GLVec3> vertices = {
            GLVec3{-0.5, -0.5, +0.0},
            GLVec3{-0.3, +0.25, +0.0},
            GLVec3{+0.5, +0.5, +0.0},
            GLVec3{+0.0, -0.75, +0.0},
            GLVec3{+0.5, -0.5, +0.0},
    };

    auto vbo = VBO::create(vertices);

    vao = VAO::create({{0, vbo}});
}

void Viewer::init_ogl() {
    bezierCurveShaderProgram = ShaderProgram::create({
        {
            GL_VERTEX_SHADER,
            readFile("shaders/basic_vert.glsl")
        }, {
            GL_TESS_CONTROL_SHADER,
            readFile("shaders/bezier_curves/tessCont.glsl")
        }, {
            GL_TESS_EVALUATION_SHADER,
            readFile("shaders/bezier_curves/tessEval.glsl")
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


    init_vao();

    set_scene_center(GLVec3(0, 0, 0));
    set_scene_radius(3.0);

    glClearColor(0., 0., 0., 1.);
    glClear(GL_COLOR_BUFFER_BIT);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Viewer::draw_ogl() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPointSize(pointsSize);

    const auto& cpCount = vao->length();

    bezierCurveShaderProgram->bind();

    set_uniform_value("uColor", GLVec4(color));
    set_uniform_value("uOuterLevel1", static_cast<GLfloat>(outerTesselationLevel1));
    set_uniform_value("uCPCount", static_cast<GLuint>(cpCount));

    vao->bind();
    glPatchParameteri(GL_PATCH_VERTICES, cpCount);
    glDrawArrays(GL_PATCHES, 0, cpCount);
    vao->unbind();

    bezierCurveShaderProgram->unbind();


    pointsShaderProgram->bind();
    vao->bind();

    set_uniform_value("uColor", GLVec4({0., 1., 0., .3}));
    glDrawArrays(GL_LINE_STRIP, 0, cpCount);

    set_uniform_value("uColor", GLVec4({0., 1., 0., 1.}));
    glDrawArrays(GL_POINTS, 0, cpCount);

    vao->unbind();
    pointsShaderProgram->unbind();
}

void Viewer::interface_ogl() {
    bool ui_tesselation_level_show = true;
    ImGui::Begin("Parameters", &ui_tesselation_level_show);

    if (ImGui::TreeNode("Rendering")) {
        ImGui::ColorEdit4("Color", color);
        ImGui::SliderInt("CP Size", &pointsSize, 0, 40);

        ImGui::TreePop();
    }

    if (ImGui::TreeNode("Parameters")) {
        ImGui::SliderInt(
            "Points Count",
            &outerTesselationLevel1,
            0, 50
        );

        ImGui::TreePop();
    }

    ImGui::End();
}

int main() {
    Viewer viewer;
    viewer.launch3d();

    return 0;
}
