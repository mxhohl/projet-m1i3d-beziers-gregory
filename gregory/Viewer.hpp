#ifndef BEZIER_VIEWER_HPP
#define BEZIER_VIEWER_HPP

#include <easycppogl_src/gl_viewer.h>

#include "GregoryRectSurface.hpp"

using namespace EZCOGL;

class Viewer : public GLViewer {
public:
    Viewer();
    void init_ogl() override;
    void draw_ogl() override;
    void interface_ogl() override;

private:
    std::shared_ptr<GregoryRectSurface> gregoryRectSurface;
};


#endif //BEZIER_VIEWER_HPP
