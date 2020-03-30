#include "BezierRectSurface.hpp"

BezierRectSurface::BezierRectSurface() : BezierRectSurface(0) {}

BezierRectSurface::BezierRectSurface(size_t orderUV) :
        BezierRectSurface(orderUV, orderUV) {}

BezierRectSurface::BezierRectSurface(size_t orderU, size_t orderV) :
        orderU(orderU),
        orderV(orderV),
        ctrlPoints(orderU * orderV),
        RenderableBezier(ctrlPoints) {}

BezierRectSurface::BezierRectSurface(
        std::initializer_list<std::initializer_list<GLVec3>> ctrlPoints) :
            RenderableBezier() {
    size_t countU = 0;
    size_t countV = 0;
    for (auto ctrlPoint : ctrlPoints) {
        ++countU;
        size_t count = 0;
        for (auto itV = ctrlPoint.begin(); itV != ctrlPoint.end(); ++itV) {
            ++count;
        }
        if (count > countV) {
            countV = count;
        }
    }

    orderU = countU - 1;
    orderV = countV - 1;
    this->ctrlPoints.resize(orderU * orderV, {0, 0, 0});

    size_t u = 0;
    for (auto itU = ctrlPoints.begin(); itU != ctrlPoints.end(); ++itU, ++u) {
        size_t v = 0;
        for (auto itV = itU->begin(); itV != itU->end(); ++itV, ++v) {
            this->ctrlPoints[u * countV + v] = *itV;
        }
        for (; v < countV; ++v) {
            this->ctrlPoints[u * countV + v] = {0, 0, 0};
        }
    }
}

BezierRectSurface::BezierRectSurface(const BezierRectSurface&) = default;

BezierRectSurface::BezierRectSurface(BezierRectSurface&&) noexcept = default;

size_t BezierRectSurface::getOrderU() const {
    return orderU;
}

size_t BezierRectSurface::getOrderV() const {
    return orderV;
}

std::pair<size_t, size_t> BezierRectSurface::getOrders() const {
    return std::make_pair(orderU, orderV);
}

const GLVec3& BezierRectSurface::getCtrlPoint(size_t u, size_t v) const {
    return ctrlPoints[u * (orderV + 1) + v];
}

void BezierRectSurface::setCtrlPoint(size_t u, size_t v, const GLVec3 &cp) {
    ctrlPoints[u * (orderV + 1) + v] = cp;
    updatePoints(ctrlPoints);
}
