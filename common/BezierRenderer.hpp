#ifndef BEZIERRENDERER_HPP
#define BEZIERRENDERER_HPP

#include "BezierCurve.hpp"

#include <easycppogl_src/shader_program.h>

class BezierRenderer {
public:
    BezierRenderer();

    void clear();
    void render(const std::shared_ptr<BezierCurve>& curve,
                float precision = 50.f);

    void setCtrlPointsSize(float size);
    float getCtrlPointsSize() const;

    void setCurveColor(const GLColor& color);
    GLColor getCurveColor() const;

    void setCtrlPointsColor(const GLColor& color);
    GLColor getCtrlPointsColor() const;

private:
    std::shared_ptr<ShaderProgram> curveSP;
    std::shared_ptr<ShaderProgram> cpSP;

    GLColor curveColor;
    GLColor ctrlPointsColor;
};

#endif // BEZIERRENDERER_HPP
