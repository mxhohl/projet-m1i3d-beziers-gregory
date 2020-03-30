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

    rectSurfaceSP = ShaderProgram::create({
        {
            GL_VERTEX_SHADER,
            readFile("shaders/basicTransformable_vert.glsl")
        }, {
            GL_TESS_CONTROL_SHADER,
            readFile("shaders/bezier_surface_rect/tessCont.glsl")
        }, {
            GL_TESS_EVALUATION_SHADER,
            readFile("shaders/bezier_surface_rect/tessEval.glsl")
        }, {
            GL_FRAGMENT_SHADER,
            readFile("shaders/basic_frag.glsl")
        }
    }, "");


    ctrlPoint2DSP = ShaderProgram::create({
        {
            GL_VERTEX_SHADER,
            readFile("shaders/basic_vert.glsl")
        }, {
            GL_FRAGMENT_SHADER,
            readFile("shaders/basic_frag.glsl")
        }
    }, "");

    ctrlPointSP = ShaderProgram::create({
        {
                GL_VERTEX_SHADER,
                readFile("shaders/basicTransformable_vert.glsl")
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
    const size_t cpCount = curve->getOrder() +1;

    if (cpCount < 1) {
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

    ctrlPoint2DSP->bind();
    curve->vao->bind();
    glDrawArrays(GL_LINE_STRIP, 0, cpCount);
    glDrawArrays(GL_POINTS, 0, cpCount);
    curve->vao->unbind();
    ctrlPoint2DSP->unbind();
}

void BezierRenderer::render(const std::shared_ptr<BezierRectSurface> &surface,
                            float precision) {
    const size_t cpCountU = surface->getOrderU() + 1;
    const size_t cpCountV = surface->getOrderV() + 1;
    const size_t& cpCount = cpCountU * cpCountV;

    if (cpCount < 1) {
        return;
    }

    /* TODO: MVP matrix */

    rectSurfaceSP->bind();
    set_uniform_value("uLevel", static_cast<GLfloat>(precision));
    set_uniform_value("uCPUCount", static_cast<GLuint>(cpCountU));
    set_uniform_value("uCPVCount", static_cast<GLuint>(cpCountV));
    surface->vao->bind();
    glPatchParameteri(GL_PATCH_VERTICES, cpCount);
    glDrawArrays(GL_PATCHES, 0, cpCount);
    surface->vao->unbind();
    rectSurfaceSP->unbind();

    ctrlPointSP->bind();
    surface->vao->bind();
    glDrawArrays(GL_POINTS, 0, cpCount);
    surface->vao->unbind();
    ctrlPointSP->unbind();
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
    ctrlPoint2DSP->bind();
    set_uniform_value("uColor", ctrlPointsColor);
    ctrlPoint2DSP->unbind();
}

GLColor BezierRenderer::getCtrlPointsColor() const {
    return ctrlPointsColor;
}

