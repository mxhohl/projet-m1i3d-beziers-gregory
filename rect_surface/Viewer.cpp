#include "Viewer.hpp"

#include <random>

Viewer::Viewer() :
        vao(nullptr),
        dimU(0),
        dimV(0),
        drawMode(DrawMode::Fill),
        tesselationLevel(1),
        color{1., 0., 0., 1.},
        pointsSize(10) {
}

void Viewer::init_bezierSurfaces_vao() {
    constexpr size_t dimensionU = 6;
    constexpr size_t dimensionV = 4;

    std::vector<GLVec3> vertices;
    vertices.reserve(dimensionU * dimensionV);

    std::default_random_engine generator(
            std::chrono::system_clock::now().time_since_epoch().count()
    );
    std::uniform_real_distribution<float> distribution(0.f, 2.f);
    auto rand = std::bind(distribution, generator);

    constexpr float offset = 1.f;
    constexpr float uHalfSize = (offset * dimensionU) / 2.f;
    constexpr float vHalfSize = (offset * dimensionV) / 2.f;

    for (size_t u = 0; u < dimensionU; ++u) {
        for (size_t v = 0; v < dimensionV; ++v) {
            vertices.emplace_back(
                    (u * offset) - uHalfSize,
                    (v * offset) - vHalfSize,
                    rand()
            );
        }
    }

    auto vbo = VBO::create(vertices);
    vao = VAO::create({{0, vbo}});
    dimU = dimensionU;
    dimV = dimensionV;
}

void Viewer::init_ogl() {

    bezierSurfaceShaderProgram = ShaderProgram::create({
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

    transformablePointsShaderProgram = ShaderProgram::create({
                                                                     {
                                                                             GL_VERTEX_SHADER,
                                                                             readFile("shaders/basicTransformable_vert.glsl")
                                                                     }, {
                                                                             GL_FRAGMENT_SHADER,
                                                                             readFile("shaders/basic_frag.glsl")
                                                                     }
                                                             }, "");

    init_bezierSurfaces_vao();

    set_scene_center(GLVec3(0, 0, 0));
    set_scene_radius(3.0);

    glClearColor(0., 0., 0., 1.);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Viewer::draw_ogl() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPointSize(pointsSize);

    glPolygonMode(GL_FRONT_AND_BACK, gl_draw_mode(drawMode));

    const auto& cpCount = vao->length();

    const auto& projMat = this->get_projection_matrix();
    const auto& mvMat = this->get_modelview_matrix();

    bezierSurfaceShaderProgram->bind();

    set_uniform_value("projMatrix", projMat);
    set_uniform_value("mvMatrix", mvMat);
    set_uniform_value("uColor", GLVec4(color));

    set_uniform_value("uLevel", static_cast<GLfloat>(tesselationLevel));

    set_uniform_value("uCPUCount", dimU);
    set_uniform_value("uCPVCount", dimV);

    vao->bind();
    glPatchParameteri(GL_PATCH_VERTICES, cpCount);
    glDrawArrays(GL_PATCHES, 0, cpCount);
    vao->unbind();

    bezierSurfaceShaderProgram->unbind();


    transformablePointsShaderProgram->bind();

    set_uniform_value("projMatrix", projMat);
    set_uniform_value("mvMatrix", mvMat);
    set_uniform_value("uColor", GLVec4({0., 1., 0., 1.}));

    vao->bind();
    glDrawArrays(GL_POINTS, 0, cpCount);
    vao->unbind();

    transformablePointsShaderProgram->unbind();
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
