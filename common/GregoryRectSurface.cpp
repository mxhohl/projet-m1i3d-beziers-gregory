#include <random>
#include "GregoryRectSurface.hpp"
#include "utils.hpp"

GregoryRectSurface::GregoryRectSurface() {
    ctrlPointSP = ShaderProgram::create({
        {
            GL_FRAGMENT_SHADER,
            readFile("shaders/basic_frag.glsl")
        },
        {
            GL_VERTEX_SHADER,
            readFile("shaders/basicTransformable_vert.glsl")
        }
    }, "");

    surfaceSP = ShaderProgram::create({
        {
            GL_FRAGMENT_SHADER,
            readFile("shaders/basic_fragUV.glsl")
        }, {
            GL_TESS_CONTROL_SHADER,
            readFile("shaders/gregory_surface_rect/tessCont.glsl")
        }, {
            GL_TESS_EVALUATION_SHADER,
            readFile("shaders/gregory_surface_rect/tessEval.glsl")
        }, {
            GL_VERTEX_SHADER,
            readFile("shaders/basicMV_vert.glsl")
        }
    }, "");

    std::default_random_engine generator;
    std::uniform_real_distribution<float> distribution(-.5, +.5);
    auto getY = std::bind(distribution, generator);

    ctrlPoints = {
            {0, 0, getY()}, // P00
            {1, 0, getY()}, // P01
            {2, 0, getY()}, // P02
            {3, 0, getY()}, // P03

            {0, 1, getY()}, // P10
            {.5, 1, getY()}, // P11v
            {2.5, 1, getY()}, // P12v
            {3, 1, getY()}, // P13

            {0, 2, getY()}, // P20
            {.5, 2, getY()}, // P21v
            {2.5, 2, getY()}, // P22v
            {3, 2, getY()}, // P23

            {0, 3, getY()}, // P30
            {1, 3, getY()}, // P31
            {2, 3, getY()}, // P32
            {3, 3, getY()}, // P33

            {1, .5, getY()}, // P11u
            {2, .5, getY()}, // P12u
            {1, 2.5, getY()}, // P21u
            {2, 2.5, getY()}, // P22u
    };

    ctrlPointsLines = {
            // bottom side
            0, 1,
            1, 2,
            2, 3,
            // right side
            3, 7,
            7, 11,
            11, 15,
            // top side
            15, 14,
            14, 13,
            13, 12,
            //left side
            12, 8,
            8, 4,
            4, 0,
            // center v
            4, 5,
            6, 7,
            8, 9,
            10, 11,
            //center u
            1, 16,
            2, 17,
            13, 18,
            14, 19
    };

    const auto scale = 1.f / 3.f;
    const auto offset = GLVec3{-1, -1, 0} * scale;
    for (auto& ctrlPoint : ctrlPoints) {
        ctrlPoint = ctrlPoint * scale + offset;
    }

    vbo = VBO::create(ctrlPoints);
    vao = VAO::create({{0, vbo}});
    ctrlPointsEbo = EBO::create(ctrlPointsLines);

    vao->bind();
    ctrlPointsEbo->bind();
    vao->unbind();

    glPointSize(7);
    glEnable(GL_DEPTH_TEST);
}

void GregoryRectSurface::draw() {
    glClearColor(0., 0., 0., 1.);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    ctrlPointSP->bind();
    set_uniform_value("uColor", GLColor{255, 0, 0, 255});
    vao->bind();
    glDrawArrays(GL_POINTS, 0, ctrlPoints.size());
    glDrawElements(GL_LINES, ctrlPointsLines.size(), GL_UNSIGNED_INT, 0);
    vao->unbind();
    ctrlPointSP->unbind();

    surfaceSP->bind();
    set_uniform_value("uLevel", 50.f);
    vao->bind();
    glPatchParameteri(GL_PATCH_VERTICES, ctrlPoints.size());
    glDrawArrays(GL_PATCHES, 0, ctrlPoints.size());
    vao->unbind();
    surfaceSP->unbind();
}

const GLMat4 &GregoryRectSurface::getProjectionMatrix() const {
    return projectionMatrix;
}

void GregoryRectSurface::setProjectionMatrix(const GLMat4& mat) {
    projectionMatrix = mat;

    ctrlPointSP->bind();
    set_uniform_value("projMatrix", projectionMatrix);
    ctrlPointSP->unbind();

    surfaceSP->bind();
    set_uniform_value("projMatrix", projectionMatrix);
    surfaceSP->unbind();
}

const GLMat4 &GregoryRectSurface::getModelViewMatrix() const {
    return modelViewMatrix;
}

void GregoryRectSurface::setModelViewMatrix(const GLMat4& mat) {
    modelViewMatrix = mat;

    ctrlPointSP->bind();
    set_uniform_value("mvMatrix", modelViewMatrix);
    ctrlPointSP->unbind();

    surfaceSP->bind();
    set_uniform_value("mvMatrix", modelViewMatrix);
    surfaceSP->unbind();
}
