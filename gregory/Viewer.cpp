#include "Viewer.hpp"

Viewer::Viewer() {
    gregoryRectSurface = std::make_shared<GregoryRectSurface>();

    set_scene_center(GLVec3(0, 0, 0));
    set_scene_radius(3.0);
}

void Viewer::init_ogl() {

}

void Viewer::draw_ogl() {
    gregoryRectSurface->setProjectionMatrix(get_projection_matrix());
    gregoryRectSurface->setModelViewMatrix(get_modelview_matrix());
    gregoryRectSurface->draw();
}

void Viewer::interface_ogl() {

}
