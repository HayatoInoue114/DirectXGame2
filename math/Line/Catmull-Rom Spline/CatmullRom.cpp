#include "CatmullRom.h"

Vector3 CatmullRomSpline::GetPoint(float t)
{
    size_t l = _controlPoints.size();
    float progress = (l - 1) * t;
    size_t i = std::floor(progress);
    float weight = progress - i;

    if (std::abs(weight) < 0.001f && i >= l - 1) {
        i = l - 2;
        weight = 1.0f;
    }

    const Vector3& p0 = _controlPoints[i];
    const Vector3& p1 = _controlPoints[i + 1];

    Vector3 p2;
    if (i > 0) {
        p2 = { 0.5f * (_controlPoints[i + 1].x - _controlPoints[i - 1].x), 0.5f * (_controlPoints[i + 1].y - _controlPoints[i - 1].y), 0.5f * (_controlPoints[i + 1].z - _controlPoints[i - 1].z) };
    }
    else {
        p2 = { _controlPoints[i + 1].x - _controlPoints[i].x, _controlPoints[i + 1].y - _controlPoints[i].y, _controlPoints[i + 1].z - _controlPoints[i].z };
    }

    Vector3 p3;
    if (i < l - 2) {
        p3 = { 0.5f * (_controlPoints[i + 2].x - _controlPoints[i].x), 0.5f * (_controlPoints[i + 2].y - _controlPoints[i].y), 0.5f * (_controlPoints[i + 2].z - _controlPoints[i].z) };
    }
    else {
        p3 = { _controlPoints[i + 1].x - _controlPoints[i].x, _controlPoints[i + 1].y - _controlPoints[i].y, _controlPoints[i + 1].z - _controlPoints[i].z };
    }

    return _poly.GetPoint(p0, p1, p2, p3, weight);
}
