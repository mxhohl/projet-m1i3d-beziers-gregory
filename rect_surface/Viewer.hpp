#ifndef BEZIER_VIEWER_HPP
#define BEZIER_VIEWER_HPP

#include "easycppogl_src/gl_viewer.h"
#include "easycppogl_src/shader_program.h"

#include "utils.hpp"
#include "BezierRenderer.hpp"
#include "BezierRectSurface.hpp"


using namespace EZCOGL;

class Viewer : public GLViewer {
public:
    Viewer();
    void init_ogl() override;
    void draw_ogl() override;
    void interface_ogl() override;

private:
    void init_ctrlPoints();

private:
    std::unique_ptr<BezierRenderer> renderer;
    std::shared_ptr<BezierRectSurface> surface;

private:
    DrawMode drawMode;

    int tesselationLevel;

    float color[4];
    int pointsSize;
};


#endif //BEZIER_VIEWER_HPP
