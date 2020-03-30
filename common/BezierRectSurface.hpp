#ifndef BEZIER_BEZIERRECTSURFACE_HPP
#define BEZIER_BEZIERRECTSURFACE_HPP

#include "RenderableBezier.hpp"

using namespace EZCOGL;

class BezierRectSurface : public RenderableBezier {
public:
    static constexpr size_t MAX_COMBINED_ORDER = 32;

public:
    BezierRectSurface();
    explicit BezierRectSurface(size_t orderUV);
    BezierRectSurface(size_t orderU, size_t orderV);
    BezierRectSurface(
            std::initializer_list<std::initializer_list<GLVec3>>  ctrlPoints);
    BezierRectSurface(const BezierRectSurface&);
    BezierRectSurface(BezierRectSurface&&) noexcept;

    size_t getOrderU() const;
    size_t getOrderV() const;
    std::pair<size_t, size_t> getOrders() const;

    const GLVec3& getCtrlPoint(size_t u, size_t v) const;
    void setCtrlPoint(size_t u, size_t v, const GLVec3& cp);

private:
    size_t orderU;
    size_t orderV;
    std::vector<GLVec3> ctrlPoints;

};


#endif //BEZIER_BEZIERRECTSURFACE_HPP
