#ifndef BEZIER_VIEWER_HPP
#define BEZIER_VIEWER_HPP

#include "easycppogl_src/gl_viewer.h"
#include "easycppogl_src/shader_program.h"

using namespace EZCOGL;

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

    float lightColor[4];
    int pointsSize;
};


#endif //BEZIER_VIEWER_HPP
