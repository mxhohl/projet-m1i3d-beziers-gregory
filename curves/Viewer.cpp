#include "Viewer.hpp"

#include "utils.hpp"

#define SELECTION_RADIUS 0.01

Viewer::Viewer() :
        movingPointIndex(-1),
        vao(nullptr),
        outerTesselationLevel1(50),
        color{1., 0., 0., 1.},
        pointsSize(10) {
}

void Viewer::init_vao() {
    controlPoints = {
            GLVec3{-0.5, -0.5, +0.0},
            GLVec3{-0.3, +0.25, +0.0},
            GLVec3{+0.5, +0.5, +0.0},
            GLVec3{+0.0, -0.75, +0.0},
            GLVec3{+0.5, -0.5, +0.0},
    };

    vbo = VBO::create(controlPoints);

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
                0, 100
        );

        ImGui::TreePop();
    }

    ImGui::End();
}

GLVec3 Viewer::windowToGlCoord(GLVec2 winCoord) {
    return {
        winCoord.x() / (width() / 2.f) - 1.f,
        -(winCoord.y() / (height() / 2.f) - 1.f),
        0.f
    };
}

void Viewer::mouse_press_ogl(int32_t button, double x, double y) {
    GLVec3 glCoord = windowToGlCoord({x, y});
    for (size_t i = 0; i < controlPoints.size(); ++i) {
        const auto& point = controlPoints[i];
        if (glCoord.x() >= point.x() - SELECTION_RADIUS 
         && glCoord.x() <= point.x() + SELECTION_RADIUS
         && glCoord.y() >= point.y() - SELECTION_RADIUS
         && glCoord.y() <= point.y() + SELECTION_RADIUS) {
             movingPointIndex = i;
             break;
        }
    }
}

void Viewer::mouse_release_ogl(int32_t button, double x, double y) {
    movingPointIndex = -1;
}

void Viewer::mouse_move_ogl(double x, double y) {
    if (movingPointIndex < 0) {
        return;
    }

    controlPoints[movingPointIndex] = windowToGlCoord({x, y});
    vbo->update(controlPoints);
}
