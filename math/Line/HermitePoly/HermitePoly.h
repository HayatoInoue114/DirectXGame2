#pragma once
#include "../../MyMath.h"

class HermitePoly {
public:
    Vector3 GetPoint(const Vector3& p0, const Vector3& p1, const Vector3& v0, const Vector3& v1, float t);

    Vector3 GetTangent(const Vector3& p0, const Vector3& p1, const Vector3& v0, const Vector3& v1, float t);
};