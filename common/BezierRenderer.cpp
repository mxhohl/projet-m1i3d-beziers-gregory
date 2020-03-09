#include "BezierRenderer.hpp"

#include "utils.hpp"

BezierRenderer::BezierRenderer() {
    curveSP = ShaderProgram::create({
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


    cpSP = ShaderProgram::create({
        {
            GL_VERTEX_SHADER,
            readFile("shaders/basic_vert.glsl")
        }, {
            GL_FRAGMENT_SHADER,
            readFile("shaders/basic_frag.glsl")
        }
    }, "");

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    clear();
}

void BezierRenderer::clear() {
    glClearColor(0., 0., 0., 1.);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void BezierRenderer::render(const std::shared_ptr<BezierCurve> &curve,
                            float precision) {
    if (curve->updated) {
        curve->vbo->update(curve->getCtrlPoints());
        curve->updated = false;
    }

    const size_t cpCount = curve->getOrder() +1;

    if (cpCount < 2) {
        return;
    }

    curveSP->bind();
    set_uniform_value("uOuterLevel1", static_cast<GLfloat>(precision));
    set_uniform_value("uCPCount", static_cast<GLuint>(cpCount));

    curve->vao->bind();
    glPatchParameteri(GL_PATCH_VERTICES, cpCount);
    glDrawArrays(GL_PATCHES, 0, cpCount);
    curve->vao->unbind();
    curveSP->unbind();

    cpSP->bind();
    curve->vao->bind();
    glDrawArrays(GL_LINE_STRIP, 0, cpCount);
    glDrawArrays(GL_POINTS, 0, cpCount);
    curve->vao->unbind();
    cpSP->unbind();
}

void BezierRenderer::setCtrlPointsSize(float size) {
    glPointSize(size);
}

float BezierRenderer::getCtrlPointsSize() const {
    float size;
    glGetFloatv(GL_POINT_SIZE, &size);
    return size;
}

void BezierRenderer::setCurveColor(const GLColor& color) {
    curveColor = color;
    curveSP->bind();
    set_uniform_value("uColor", curveColor);
    curveSP->unbind();
}

GLColor BezierRenderer::getCurveColor() const {
    return curveColor;
}

void BezierRenderer::setCtrlPointsColor(const GLColor& color) {
    ctrlPointsColor = color;
    cpSP->bind();
    set_uniform_value("uColor", ctrlPointsColor);
    cpSP->unbind();
}

GLColor BezierRenderer::getCtrlPointsColor() const {
    return ctrlPointsColor;
}

