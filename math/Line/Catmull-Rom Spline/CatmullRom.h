//#pragma once
//#include "../HermitePoly/HermitePoly.h"
//#include "../../MyMath.h"
//
//class CatmullRomSpline {
//public:
//    const std::vector<Vector3>& ControlPoints() const {
//        return _controlPoints;
//    }
//
//    CatmullRomSpline(const HermitePoly& poly) : _poly(poly) {}
//
//    Vector3 GetPoint(float t);
//
//private:
//    HermitePoly _poly;
//    std::vector<Vector3> _controlPoints = {
//        { 0.0f, 0.0f, 0.0f },
//        { 1.0f, 0.0f, 0.0f },
//        { 1.0f, 0.0f, 1.0f },
//        { 1.0f, 0.0f, 2.0f }
//    };
//};