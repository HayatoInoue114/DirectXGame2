//#include "HermitePoly.h"
//
//Vector3 HermitePoly::GetPoint(const Vector3& p0, const Vector3& p1, const Vector3& v0, const Vector3& v1, float t)
//{
//    Vector3 c0 = { 2.0f * p0.x + -2.0f * p1.x + v0.x + v1.x, 2.0f * p0.y + -2.0f * p1.y + v0.y + v1.y, 2.0f * p0.z + -2.0f * p1.z + v0.z + v1.z };
//    Vector3 c1 = { -3.0f * p0.x + 3.0f * p1.x + -2.0f * v0.x - v1.x, -3.0f * p0.y + 3.0f * p1.y + -2.0f * v0.y - v1.y, -3.0f * p0.z + 3.0f * p1.z + -2.0f * v0.z - v1.z };
//    Vector3 c2 = v0;
//    Vector3 c3 = p0;
//
//    float t2 = t * t;
//    float t3 = t2 * t;
//    return { c0.x * t3 + c1.x * t2 + c2.x * t + c3.x, c0.y * t3 + c1.y * t2 + c2.y * t + c3.y, c0.z * t3 + c1.z * t2 + c2.z * t + c3.z };
//}
//
//Vector3 HermitePoly::GetTangent(const Vector3& p0, const Vector3& p1, const Vector3& v0, const Vector3& v1, float t)
//{
//    Vector3 c0 = { 6.0f * p0.x - 6.0f * p1.x + 3.0f * v0.x + 3.0f * v1.x, 6.0f * p0.y - 6.0f * p1.y + 3.0f * v0.y + 3.0f * v1.y, 6.0f * p0.z - 6.0f * p1.z + 3.0f * v0.z + 3.0f * v1.z };
//    Vector3 c1 = { -6.0f * p0.x + 6.0f * p1.x - 4.0f * v0.x - 2.0f * v1.x, -6.0f * p0.y + 6.0f * p1.y - 4.0f * v0.y - 2.0f * v1.y, -6.0f * p0.z + 6.0f * p1.z - 4.0f * v0.z - 2.0f * v1.z };
//    Vector3 c2 = v0;
//
//    float t2 = t * t;
//    return { c0.x * t2 + c1.x * t + c2.x, c0.y * t2 + c1.y * t + c2.y, c0.z * t2 + c1.z * t + c2.z };
//}
