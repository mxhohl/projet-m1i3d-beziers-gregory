#include "BezierCurve.hpp"

BezierCurve::BezierCurve() : BezierCurve(0) {}

BezierCurve::BezierCurve(size_t order) :
        ctrlPoints(order +1, {0, 0, 0}),
        RenderableBezier(ctrlPoints) {}

BezierCurve::BezierCurve(const std::vector<GLVec3>& ctrlPoints) :
        ctrlPoints(ctrlPoints),
        RenderableBezier(ctrlPoints) {}

BezierCurve::BezierCurve(std::initializer_list<GLVec3> ctrlPoints) :
        ctrlPoints(ctrlPoints),
        RenderableBezier(ctrlPoints) {}

BezierCurve::BezierCurve(const BezierCurve&) = default;
BezierCurve::BezierCurve(BezierCurve&&) = default;

size_t BezierCurve::getOrder() const {
    return ctrlPoints.size() -1;
}

void BezierCurve::elevateOrder(size_t d) {
    /* TODO */
    needUpdate();
}

void BezierCurve::lowerOrder(size_t d) {
    /* TODO */
    needUpdate();
}

bool BezierCurve::getClosestCtrlPoint(const GLVec3& point, size_t& i) const {
    for (i = 0; i < ctrlPoints.size(); ++i) {
        const auto& cp = ctrlPoints[i];
        if (point.x() >= cp.x() - SELECTION_RADIUS
         && point.x() <= cp.x() + SELECTION_RADIUS
         && point.y() >= cp.y() - SELECTION_RADIUS
         && point.y() <= cp.y() + SELECTION_RADIUS) {
             return true;
        }
    }
    return false;
}

const GLVec3& BezierCurve::getCtrlPoint(size_t i) const {
    return ctrlPoints[i];
}

void BezierCurve::setCtrlPoint(size_t i, const GLVec3& cp) {
    ctrlPoints[i] = cp;
    needUpdate();
}

const std::vector<GLVec3>& BezierCurve::getCtrlPoints() const {
    return ctrlPoints;
}
