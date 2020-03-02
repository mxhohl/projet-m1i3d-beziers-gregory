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
    long int movingPointIndex;
    void mouse_press_ogl(int32_t button, double x, double y) override;
	void mouse_release_ogl(int32_t button, double x, double y) override;
	void mouse_move_ogl(double x, double y) override;

private:
    void init_vao();

private:
    GLVec3 windowToGlCoord(GLVec2 winCoord);

private:
    std::shared_ptr<ShaderProgram> bezierCurveShaderProgram;
    std::shared_ptr<ShaderProgram> pointsShaderProgram;

    std::vector<GLVec3> controlPoints;
    std::shared_ptr<VBO> vbo;
    std::shared_ptr<VAO> vao;

private:
    int outerTesselationLevel1;

    float color[4];
    int pointsSize;
};


#endif //BEZIER_VIEWER_HPP
