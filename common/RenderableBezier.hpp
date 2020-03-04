#ifndef RENDERABLEBEZIER_HPP
#define RENDERABLEBEZIER_HPP

#include <easycppogl_src/vao.h>
#include <easycppogl_src/vbo.h>

//class BezierRenderer;

using namespace EZCOGL;

class RenderableBezier {
    friend class BezierRenderer;

public:
    RenderableBezier(const std::vector<GLVec3>& points) : updated(true) {
        vbo = VBO::create(points);
        vao = VAO::create({{0, vbo}});
    }

protected:
    void needUpdate() { updated = true; }

private:
    bool updated;
    std::shared_ptr<VBO> vbo;
    std::shared_ptr<VAO> vao;
};

#endif // RENDERABLEBEZIER_HPP
