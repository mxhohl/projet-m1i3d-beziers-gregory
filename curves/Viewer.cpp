#include "Viewer.hpp"

#include "utils.hpp"

Viewer::Viewer() :
        movingPointIndex(-1),
        outerTesselationLevel1(50),
        color{1., 0., 0., 1.},
        pointsSize(10) {
}


void Viewer::init_ogl() {
    bezierRenderer = std::make_unique<BezierRenderer>();
    bezierCurve = std::make_shared<BezierCurve>(std::vector<GLVec3>{
            GLVec3{-0.5, -0.5, +0.0},
            GLVec3{-0.3, +0.25, +0.0},
            GLVec3{+0.5, +0.5, +0.0},
            GLVec3{+0.0, -0.75, +0.0},
            GLVec3{+0.5, -0.5, +0.0},
    });

    bezierRenderer->setCtrlPointsColor({0, 1, 0, .5});

    set_scene_center(GLVec3(0, 0, 0));
    set_scene_radius(3.0);

    glClearColor(0., 0., 0., 1.);
    glClear(GL_COLOR_BUFFER_BIT);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Viewer::draw_ogl() {
    bezierRenderer->setCurveColor({color[0], color[1], color[2], color[3]});
    bezierRenderer->setCtrlPointsSize(static_cast<float>(pointsSize));

    bezierRenderer->clear();
    bezierRenderer->render(bezierCurve,
                           static_cast<float>(outerTesselationLevel1));
}

void Viewer::interface_ogl() {
    bool ui_tesselation_level_show = true;
    ImGui::Begin("Parameters", &ui_tesselation_level_show);

    if (ImGui::TreeNode("Rendering")) {
        ImGui::ColorEdit4("Color", color);
        ImGui::SliderInt("CP Size", &pointsSize, 0, 40);

        ImGui::TreePop();
    }

    if (ImGui::TreeNode("Parameters")) {
        ImGui::SliderInt(
                "Points Count",
                &outerTesselationLevel1,
                0, 100
        );

        ImGui::TreePop();
    }

    ImGui::End();
}

GLVec3 Viewer::windowToGlCoord(GLVec2 winCoord) {
    return {
        winCoord.x() / (width() / 2.f) - 1.f,
        -(winCoord.y() / (height() / 2.f) - 1.f),
        0.f
    };
}

void Viewer::mouse_press_ogl(int32_t button, double x, double y) {
    GLVec3 glCoord = windowToGlCoord({x, y});
    size_t pointIndex;
    if (bezierCurve->getClosestCtrlPoint(glCoord, pointIndex)) {
        movingPointIndex = pointIndex;
    } else {
        movingPointIndex = -1;
    }
}

void Viewer::mouse_release_ogl(int32_t button, double x, double y) {
    movingPointIndex = -1;
}

void Viewer::mouse_move_ogl(double x, double y) {
    if (movingPointIndex < 0) {
        return;
    }

    bezierCurve->setCtrlPoint(movingPointIndex, windowToGlCoord({x, y}));
}
