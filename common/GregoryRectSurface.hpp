#ifndef BEZIER_GREGORYRECTSURFACE_HPP
#define BEZIER_GREGORYRECTSURFACE_HPP

#include "RenderableBezier.hpp"

#include <easycppogl_src/vao.h>
#include <easycppogl_src/vbo.h>
#include <easycppogl_src/shader_program.h>
#include <easycppogl_src/ebo.h>

using namespace EZCOGL;

class GregoryRectSurface {
public:
    GregoryRectSurface();

    void draw();

    const GLMat4 &getProjectionMatrix() const;
    void setProjectionMatrix(const GLMat4& map);

    const GLMat4 &getModelViewMatrix() const;
    void setModelViewMatrix(const GLMat4& mat);

private:
    std::shared_ptr<VAO> vao;
    std::shared_ptr<VBO> vbo;

    std::vector<GLVec3> ctrlPoints;

    std::shared_ptr<EBO> ctrlPointsEbo;
    std::vector<GLuint> ctrlPointsLines;

    std::shared_ptr<ShaderProgram> ctrlPointSP;
    std::shared_ptr<ShaderProgram> surfaceSP;

    GLMat4 projectionMatrix;
    GLMat4 modelViewMatrix;
};


#endif //BEZIER_GREGORYRECTSURFACE_HPP
