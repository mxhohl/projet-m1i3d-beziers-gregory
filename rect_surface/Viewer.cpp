#include "Viewer.hpp"

#include <random>

Viewer::Viewer() :
        drawMode(DrawMode::Fill),
        tesselationLevel(1),
        color{1., 0., 0., 1.},
        pointsSize(10) {
}

void Viewer::init_ctrlPoints() {
    constexpr size_t orderU = 6;
    constexpr size_t orderV = 4;

    surface = std::make_shared<BezierRectSurface>(orderU, orderV);

    std::default_random_engine generator(
            std::chrono::system_clock::now().time_since_epoch().count()
    );
    std::uniform_real_distribution<float> distribution(0.f, 2.f);
    auto rand = std::bind(distribution, generator);

    constexpr float offset = 1.f;
    constexpr float uHalfSize = (offset * orderU) / 2.f;
    constexpr float vHalfSize = (offset * orderV) / 2.f;

    for (size_t u = 0; u < orderU; ++u) {
        for (size_t v = 0; v < orderV; ++v) {
            surface->setCtrlPoint(u, v, {
                    (u * offset) - uHalfSize,
                    (v * offset) - vHalfSize,
                    rand()
            });
        }
    }
}

void Viewer::init_ogl() {
    renderer = std::make_unique<BezierRenderer>();
    renderer->setCtrlPointsColor({0, 1, 0, .5});

    init_ctrlPoints();

    set_scene_center(GLVec3(0, 0, 0));
    set_scene_radius(3.0);

    renderer->clear();
}

void Viewer::draw_ogl() {
    renderer->setCurveColor({color[0], color[1], color[2], color[3]});
    renderer->setCtrlPointsSize(static_cast<float>(pointsSize));

    renderer->clear();
    renderer->render(surface, static_cast<float>(tesselationLevel));
}

void Viewer::interface_ogl() {
    bool ui_tesselation_level_show = true;
    ImGui::Begin("Parameters", &ui_tesselation_level_show);

    if (ImGui::TreeNode("Rendering")) {
        ImGui::SliderInt(
                ("Draw Mode - " + to_string(drawMode)).c_str(),
                reinterpret_cast<int*>(&drawMode),
                0, 2
        );
        ImGui::ColorEdit4("Color", color);
        ImGui::SliderInt("CP Size", &pointsSize, 0, 40);

        ImGui::TreePop();
    }

    if (ImGui::TreeNode("Parameters")) {
        ImGui::SliderInt(
                "Tesselation Level",
                &tesselationLevel,
                1, 50
        );

        ImGui::TreePop();
    }

    ImGui::End();
}
