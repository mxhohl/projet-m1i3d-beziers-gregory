#ifndef BEZIERCURVE_HPP
#define BEZIERCURVE_HPP

#include "RenderableBezier.hpp"

#include <vector>
#include <easycppogl_src/gl_eigen.h>

using namespace EZCOGL;

class BezierCurve : public RenderableBezier {
public:
    static constexpr size_t MAX_ORDER = 32;
    static constexpr float SELECTION_RADIUS = 0.01;

public:
    BezierCurve();
    explicit BezierCurve(size_t order);
    explicit BezierCurve(const std::vector<GLVec3>& ctrlPoints);
    BezierCurve(std::initializer_list<GLVec3> ctrlPoints);
    BezierCurve(const BezierCurve&);
    BezierCurve(BezierCurve&&) noexcept;

    size_t getOrder() const;
    void elevateOrder(size_t d = 1);
    void lowerOrder(size_t d = 1);

    bool getClosestCtrlPoint(const GLVec3& point, size_t& i) const;
    const GLVec3& getCtrlPoint(size_t i) const;
    void setCtrlPoint(size_t i, const GLVec3& cp);

    const std::vector<GLVec3>& getCtrlPoints() const;

private:
    std::vector<GLVec3> ctrlPoints;
};

#endif // BEZIERCURVE_HPP
