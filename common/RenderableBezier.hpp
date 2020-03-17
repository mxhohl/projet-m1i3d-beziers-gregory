#ifndef RENDERABLEBEZIER_HPP
#define RENDERABLEBEZIER_HPP

#include <easycppogl_src/vao.h>
#include <easycppogl_src/vbo.h>

using namespace EZCOGL;

class RenderableBezier {
    friend class BezierRenderer;

public:
    RenderableBezier() {
        vbo = VBO::create({});
        vao = VAO::create({{0, vbo}});
    }

    explicit RenderableBezier(const std::vector<GLVec3>& points) {
        vbo = VBO::create(points);
        vao = VAO::create({{0, vbo}});
    }

    void updatePoints(const std::vector<GLVec3>& points) {
        vbo->update(points);
    }

private:
    std::shared_ptr<VBO> vbo;
    std::shared_ptr<VAO> vao;
};

#endif // RENDERABLEBEZIER_HPP
