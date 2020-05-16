#include "Viewer.hpp"

Viewer::Viewer() :
        drawMode(DrawMode::Fill),
        tesselationLevel(20),
        pointsSize(10) {
    gregoryRectSurface = std::make_shared<GregoryRectSurface>();

    set_scene_center(GLVec3(0, 0, 0));
    set_scene_radius(3.0);
}

void Viewer::init_ogl() {

}

void Viewer::draw_ogl() {
    gregoryRectSurface->setProjectionMatrix(get_projection_matrix());
    gregoryRectSurface->setModelViewMatrix(get_modelview_matrix());
    gregoryRectSurface->setPointsSize(pointsSize);
    gregoryRectSurface->setTesselationLevel(tesselationLevel);

    gregoryRectSurface->draw();
}

void Viewer::interface_ogl() {
    bool ui_show = true;
    ImGui::Begin("Parameters", &ui_show);

    if (ImGui::TreeNode("Rendering")) {
        ImGui::SliderInt(
                ("Draw Mode - " + to_string(drawMode)).c_str(),
                reinterpret_cast<int*>(&drawMode),
                0, 2
        );
        ImGui::SliderInt("CP Size", &pointsSize, 0, 40);

        ImGui::TreePop();
    }

    if (ImGui::TreeNode("Parameters")) {
        ImGui::SliderInt("Tesselation Level", &tesselationLevel, 1, 50);

        ImGui::TreePop();
    }

    ImGui::End();
}
