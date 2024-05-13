#pragma once
#include "../MT/MT.h"


/// -------------------------------------------------------------------------
/// 2次元ベクトル
/// -------------------------------------------------------------------------
inline Vector2 operator+(const Vector2& v) {
	return { +v.x, +v.y };
}
inline Vector2 operator-(const Vector2& v) {
	return { -v.x, -v.y };
}
inline constexpr bool operator==(const Vector2& rhs, const float& val) {
	return val == rhs.x || val == rhs.y;
}
inline constexpr bool operator!=(const Vector2& rhs, const float& val) {
	return val != rhs.x || val != rhs.y;
}

// 加算
inline Vector2 operator+(const Vector2& v, const float& n) {
	return { v.x + n, v.y + n };
}
inline Vector2 operator+(const float& n, const Vector2& v) {
	return { v.x + n, v.y + n };
}
inline Vector2 operator+(const Vector2& v1, const Vector2& v2) {
	return { v1.x + v2.x, v1.y + v2.y };
}
inline Vector2& operator+=(Vector2& v1, const Vector2& v2) {
	return v1 = v1 + v2;
}

// 減算
inline Vector2 operator-(const Vector2& v, const float& n) {
	return { v.x - n, v.y - n };
}
inline Vector2 operator-(const float& n, const Vector2& v) {
	return { v.x - n, v.y - n };
}
inline Vector2 operator-(const Vector2& v1, const Vector2& v2) {
	return { v1.x - v2.x, v1.y - v2.y };
}
inline Vector2& operator-=(Vector2& v1, const Vector2& v2) {
	return v1 = v1 - v2;
}

// 乗算
inline Vector2 operator*(const Vector2& v, const float& s) {
	return { v.x * s, v.y * s };
}
inline Vector2 operator*(const float& s, const Vector2& v) {
	return { v.x * s, v.y * s };
}
inline Vector2& operator*=(Vector2& v, const float& s) {
	return v = v * s;
}
inline Vector2 operator*(const Vector2& v1, const Vector2& v2) {
	return { v1.x * v2.x, v1.y * v2.y };
}
inline Vector2& operator*=(Vector2& v1, const Vector2& v2) {
	return v1 = v1 * v2;
}

// 除算
inline Vector2 operator/(const Vector2& v, const float& s) {
	return { v.x / s, v.y / s };
}
inline Vector2 operator/(const float& s, const Vector2& v) {
	return { v.x / s, v.y / s };
}
inline Vector2 operator/=(Vector2& v, const float& s) {
	return v = v / s;
}
inline Vector2 operator/(const Vector2& v1, const Vector2& v2) {
	return { v1.x / v2.x, v1.y / v2.y };
}
inline Vector2& operator/=(Vector2& v1, const Vector2& v2) {
	return v1 = v1 / v2;
}





/// -------------------------------------------------------------------------
/// 3次元ベクトル
/// -------------------------------------------------------------------------
inline Vector3 operator+(const Vector3& v) {
	return { +v.x, +v.y, +v.z };
}
inline Vector3 operator-(const Vector3& v) {
	return { -v.x, -v.y, -v.z };
}
inline constexpr bool operator==(const Vector3& rhs, const float& val) {
	return val == rhs.x || val == rhs.y || val == rhs.z;
}
inline constexpr bool operator!=(const Vector3& rhs, const float& val) {
	return val != rhs.x || val != rhs.y || val != rhs.z;
}

// 加算
inline Vector3 operator+(const Vector3& v, const float& n) {
	return { v.x + n, v.y + n, v.z + n };
}
inline Vector3 operator+(const float& n, const Vector3& v) {
	return { v.x + n, v.y + n, v.z + n };
}
inline Vector3 operator+(const Vector3& v1, const Vector3& v2) {
	return { v1.x + v2.x, v1.y + v2.y, v1.z + v2.z };
}
inline Vector3& operator+=(Vector3& v1, const Vector3& v2) {
	return v1 = v1 + v2;
}

// 減算
inline Vector3 operator-(const Vector3& v, const float& n) {
	return { v.x - n, v.y - n, v.z - n };
}
inline Vector3 operator-(const float& n, const Vector3& v) {
	return { v.x - n, v.y - n, v.z - n };
}
inline Vector3 operator-(const Vector3& v1, const Vector3& v2) {
	return { v1.x - v2.x, v1.y - v2.y, v1.z - v2.z };
}
inline Vector3& operator-=(Vector3& v1, const Vector3& v2) {
	return v1 = v1 - v2;
}

// 乗算
inline Vector3 operator*(const Vector3& v, const float& s) {
	return { v.x * s, v.y * s, v.z * s };
}
inline Vector3 operator*(const float& s, const Vector3& v) {
	return { v.x * s, v.y * s, v.z * s };
}
inline Vector3& operator*=(Vector3& v, const float& s) {
	return v = v * s;
}
inline Vector3 operator*(const Vector3& v1, const Vector3& v2) {
	return { v1.x * v2.x, v1.y * v2.y, v1.z * v2.z };
}
inline Vector3& operator*=(Vector3& v1, const Vector3& v2) {
	return v1 = v1 * v2;
}

// 除算
inline Vector3 operator/(const Vector3& v, const float& s) {
	return { v.x / s, v.y / s, v.z / s };
}
inline Vector3 operator/(const float& s, const Vector3& v) {
	return { v.x / s, v.y / s, v.z / s };
}
inline Vector3 operator/=(Vector3& v, const float& s) {
	return v = v / s;
}
inline Vector3 operator/(const Vector3& v1, const Vector3& v2) {
	return { v1.x / v2.x, v1.y / v2.y, v1.z / v2.z };
}
inline Vector3& operator/=(Vector3& v1, const Vector3& v2) {
	return v1 = v1 / v2;
}





/// -------------------------------------------------------------------------
/// 4次元ベクトル
/// -------------------------------------------------------------------------
inline Vector4 operator+(const Vector4& v) {
	return { +v.x, +v.y, +v.z, +v.w };
}
inline Vector4 operator-(const Vector4& v) {
	return { -v.x, -v.y, -v.z, -v.w };
}
inline constexpr bool operator==(const Vector4& rhs, const float& val) {
	return val == rhs.x || val == rhs.y || val == rhs.z || val == rhs.w;
}
inline constexpr bool operator!=(const Vector4& rhs, const float& val) {
	return val != rhs.x || val != rhs.y || val != rhs.z || val != rhs.w;
}

// 加算
inline Vector4 operator+(const Vector4& v, const float& n) {
	return { v.x + n, v.y + n, v.z + n, v.w + n };
}
inline Vector4 operator+(const float& n, const Vector4& v) {
	return { v.x + n, v.y + n, v.z + n, v.w + n };
}
inline Vector4 operator+(const Vector4& v1, const Vector4& v2) {
	return { v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w };
}
inline Vector4& operator+=(Vector4& v1, const Vector4& v2) {
	return v1 = v1 + v2;
}

// 減算
inline Vector4 operator-(const Vector4& v, const float& n) {
	return { v.x - n, v.y - n, v.z - n, v.w - n };
}
inline Vector4 operator-(const float& n, const Vector4& v) {
	return { v.x - n, v.y - n, v.z - n, v.w - n };
}
inline Vector4 operator-(const Vector4& v1, const Vector4& v2) {
	return { v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w };
}
inline Vector4& operator-=(Vector4& v1, const Vector4& v2) {
	return v1 = v1 - v2;
}

// 乗算
inline Vector4 operator*(const Vector4& v, const float& s) {
	return { v.x * s, v.y * s, v.z * s, v.w * s };
}
inline Vector4 operator*(const float& s, const Vector4& v) {
	return { v.x * s, v.y * s, v.z * s, v.w * s };
}
inline Vector4& operator*=(Vector4& v, const float& s) {
	return v = v * s;
}
inline Vector4 operator*(const Vector4& v1, const Vector4& v2) {
	return { v1.x * v2.x, v1.y * v2.y, v1.z * v2.z, v1.w * v2.w };
}
inline Vector4& operator*=(Vector4& v1, const Vector4& v2) {
	return v1 = v1 * v2;
}

// 除算
inline Vector4 operator/(const Vector4& v, const float& s) {
	return { v.x / s, v.y / s, v.z / s, v.w / s };
}
inline Vector4 operator/(const float& s, const Vector4& v) {
	return { v.x / s, v.y / s, v.z / s, v.w / s };
}
inline Vector4 operator/=(Vector4& v, const float& s) {
	return v = v / s;
}
inline Vector4 operator/(const Vector4& v1, const Vector4& v2) {
	return { v1.x / v2.x, v1.y / v2.y, v1.z / v2.z, v1.w / v2.w };
}
inline Vector4& operator/=(Vector4& v1, const Vector4& v2) {
	return v1 = v1 / v2;
}





/// -------------------------------------------------------------------------
/// 3x3行列
/// -------------------------------------------------------------------------
inline Matrix3x3 operator+(const Matrix3x3& m) {
	return {
		+m.m[0][0], +m.m[0][1], +m.m[0][2],
		+m.m[1][0], +m.m[1][1], +m.m[1][2],
		+m.m[2][0], +m.m[2][1], +m.m[2][2] };
}
inline Matrix3x3 operator-(const Matrix3x3& m) {
	return {
		-m.m[0][0], -m.m[0][1], -m.m[0][2],
		-m.m[1][0], -m.m[1][1], -m.m[1][2],
		-m.m[2][0], -m.m[2][1], -m.m[2][2] };
}

// 加算
inline Matrix3x3 operator+(const Matrix3x3& m1, const Matrix3x3& m2) {
	return {
		m1.m[0][0] + m2.m[0][0], m1.m[0][1] + m2.m[0][1], m1.m[0][2] + m2.m[0][2],
		m1.m[1][0] + m2.m[1][0], m1.m[1][1] + m2.m[1][1], m1.m[1][2] + m2.m[1][2],
		m1.m[2][0] + m2.m[2][0], m1.m[2][1] + m2.m[2][1], m1.m[2][2] + m2.m[2][2] };
}
inline Matrix3x3& operator+=(Matrix3x3& m1, const Matrix3x3& m2) {
	return m1 = m1 + m2;
}

// 減算
inline Matrix3x3 operator-(const Matrix3x3& m1, const Matrix3x3& m2) {
	return {
		m1.m[0][0] - m2.m[0][0], m1.m[0][1] - m2.m[0][1], m1.m[0][2] - m2.m[0][2],
		m1.m[1][0] - m2.m[1][0], m1.m[1][1] - m2.m[1][1], m1.m[1][2] - m2.m[1][2],
		m1.m[2][0] - m2.m[2][0], m1.m[2][1] - m2.m[2][1], m1.m[2][2] - m2.m[2][2] };
}
inline Matrix3x3& operator-=(Matrix3x3& m1, const Matrix3x3& m2) {
	return m1 = m1 - m2;
}

// 乗算
inline Matrix3x3 operator*(const Matrix3x3& m, const float& s) {
	return {
		m.m[0][0] * s, m.m[0][1] * s, m.m[0][2] * s,
		m.m[1][0] * s, m.m[1][1] * s, m.m[1][2] * s,
		m.m[2][0] * s, m.m[2][1] * s, m.m[2][2] * s };
}
inline Matrix3x3 operator*(const float& s, const Matrix3x3& m) {
	return {
		m.m[0][0] * s, m.m[0][1] * s, m.m[0][2] * s,
		m.m[1][0] * s, m.m[1][1] * s, m.m[1][2] * s,
		m.m[2][0] * s, m.m[2][1] * s, m.m[2][2] * s };
}
inline Matrix3x3 operator*(const Matrix3x3& m1, const Matrix3x3& m2) {
	return {
		m1.m[0][0] * m2.m[0][0] + m1.m[0][1] * m2.m[1][0] + m1.m[0][2] * m2.m[2][0],
		m1.m[0][0] * m2.m[0][1] + m1.m[0][1] * m2.m[1][1] + m1.m[0][2] * m2.m[2][1],
		m1.m[0][0] * m2.m[0][2] + m1.m[0][1] * m2.m[1][2] + m1.m[0][2] * m2.m[2][2],

		m1.m[1][0] * m2.m[0][0] + m1.m[1][1] * m2.m[1][0] + m1.m[1][2] * m2.m[2][0],
		m1.m[1][0] * m2.m[0][1] + m1.m[1][1] * m2.m[1][1] + m1.m[1][2] * m2.m[2][1],
		m1.m[1][0] * m2.m[0][2] + m1.m[1][1] * m2.m[1][2] + m1.m[1][2] * m2.m[2][2],

		m1.m[2][0] * m2.m[0][0] + m1.m[2][1] * m2.m[1][0] + m1.m[2][2] * m2.m[2][0],
		m1.m[2][0] * m2.m[0][1] + m1.m[2][1] * m2.m[1][1] + m1.m[2][2] * m2.m[2][1],
		m1.m[2][0] * m2.m[0][2] + m1.m[2][1] * m2.m[1][2] + m1.m[2][2] * m2.m[2][2] };
}
inline Matrix3x3& operator*=(Matrix3x3& m1, const Matrix3x3& m2) {
	return m1 = m1 * m2;
}





/// -------------------------------------------------------------------------
/// 4x4行列
/// -------------------------------------------------------------------------
inline Matrix4x4 operator+(const Matrix4x4& m) {
	return {
		+m.m[0][0], +m.m[0][1], +m.m[0][2], +m.m[0][3],
		+m.m[1][0], +m.m[1][1], +m.m[1][2], +m.m[1][3],
		+m.m[2][0], +m.m[2][1], +m.m[2][2], +m.m[2][3],
		+m.m[3][0], +m.m[3][1], +m.m[3][2], +m.m[3][3] };
}
inline Matrix4x4 operator-(const Matrix4x4& m) {
	return {
		-m.m[0][0], -m.m[0][1], -m.m[0][2], -m.m[0][3],
		-m.m[1][0], -m.m[1][1], -m.m[1][2], -m.m[1][3],
		-m.m[2][0], -m.m[2][1], -m.m[2][2], -m.m[2][3],
		-m.m[3][0], -m.m[3][1], -m.m[3][2], -m.m[3][3] };
}

// 加算
inline Matrix4x4 operator+(const Matrix4x4& m1, const Matrix4x4& m2) {
	return {
		m1.m[0][0] + m2.m[0][0], m1.m[0][1] + m2.m[0][1], m1.m[0][2] + m2.m[0][2], m1.m[0][3] + m2.m[0][3],
		m1.m[1][0] + m2.m[1][0], m1.m[1][1] + m2.m[1][1], m1.m[1][2] + m2.m[1][2], m1.m[1][3] + m2.m[1][3],
		m1.m[2][0] + m2.m[2][0], m1.m[2][1] + m2.m[2][1], m1.m[2][2] + m2.m[2][2], m1.m[2][3] + m2.m[2][3],
		m1.m[3][0] + m2.m[3][0], m1.m[3][1] + m2.m[3][1], m1.m[3][2] + m2.m[3][2], m1.m[3][3] + m2.m[3][3] };
}
inline Matrix4x4& operator+=(Matrix4x4& m1, const Matrix4x4& m2) {
	return m1 = m1 + m2;
}

// 減算
inline Matrix4x4 operator-(const Matrix4x4& m1, const Matrix4x4& m2) {
	return {
		m1.m[0][0] - m2.m[0][0], m1.m[0][1] - m2.m[0][1], m1.m[0][2] - m2.m[0][2], m1.m[0][3] - m2.m[0][3],
		m1.m[1][0] - m2.m[1][0], m1.m[1][1] - m2.m[1][1], m1.m[1][2] - m2.m[1][2], m1.m[1][3] - m2.m[1][3],
		m1.m[2][0] - m2.m[2][0], m1.m[2][1] - m2.m[2][1], m1.m[2][2] - m2.m[2][2], m1.m[2][3] - m2.m[2][3],
		m1.m[3][0] - m2.m[3][0], m1.m[3][1] - m2.m[3][1], m1.m[3][2] - m2.m[3][2], m1.m[3][3] - m2.m[3][3] };
}
inline Matrix4x4& operator-=(Matrix4x4& m1, const Matrix4x4& m2) {
	return m1 = m1 - m2;
}

// 乗算
inline Matrix4x4 operator*(const Matrix4x4& m, const float& s) {
	return {
		m.m[0][0] * s, m.m[0][1] * s, m.m[0][2] * s, m.m[0][3],
		m.m[1][0] * s, m.m[1][1] * s, m.m[1][2] * s, m.m[1][3],
		m.m[2][0] * s, m.m[2][1] * s, m.m[2][2] * s, m.m[2][3],
		m.m[3][0] * s, m.m[3][1] * s, m.m[3][2] * s, m.m[3][3] };
}
inline Matrix4x4 operator*(const float& s, const Matrix4x4& m) {
	return {
		m.m[0][0] * s, m.m[0][1] * s, m.m[0][2] * s, m.m[0][3],
		m.m[1][0] * s, m.m[1][1] * s, m.m[1][2] * s, m.m[1][3],
		m.m[2][0] * s, m.m[2][1] * s, m.m[2][2] * s, m.m[2][3],
		m.m[3][0] * s, m.m[3][1] * s, m.m[3][2] * s, m.m[3][3] };
}
inline Matrix4x4 operator*(const Matrix4x4& m1, const Matrix4x4& m2) {
	return {
		m1.m[0][0] * m2.m[0][0] + m1.m[0][1] * m2.m[1][0] + m1.m[0][2] * m2.m[2][0] + m1.m[0][3] * m2.m[3][0],
		m1.m[0][0] * m2.m[0][1] + m1.m[0][1] * m2.m[1][1] + m1.m[0][2] * m2.m[2][1] + m1.m[0][3] * m2.m[3][1],
		m1.m[0][0] * m2.m[0][2] + m1.m[0][1] * m2.m[1][2] + m1.m[0][2] * m2.m[2][2] + m1.m[0][3] * m2.m[3][2],
		m1.m[0][0] * m2.m[0][3] + m1.m[0][1] * m2.m[1][3] + m1.m[0][2] * m2.m[2][3] + m1.m[0][3] * m2.m[3][3],

		m1.m[1][0] * m2.m[0][0] + m1.m[1][1] * m2.m[1][0] + m1.m[1][2] * m2.m[2][0] + m1.m[1][3] * m2.m[3][0],
		m1.m[1][0] * m2.m[0][1] + m1.m[1][1] * m2.m[1][1] + m1.m[1][2] * m2.m[2][1] + m1.m[1][3] * m2.m[3][1],
		m1.m[1][0] * m2.m[0][2] + m1.m[1][1] * m2.m[1][2] + m1.m[1][2] * m2.m[2][2] + m1.m[1][3] * m2.m[3][2],
		m1.m[1][0] * m2.m[0][3] + m1.m[1][1] * m2.m[1][3] + m1.m[1][2] * m2.m[2][3] + m1.m[1][3] * m2.m[3][3],

		m1.m[2][0] * m2.m[0][0] + m1.m[2][1] * m2.m[1][0] + m1.m[2][2] * m2.m[2][0] + m1.m[2][3] * m2.m[3][0],
		m1.m[2][0] * m2.m[0][1] + m1.m[2][1] * m2.m[1][1] + m1.m[2][2] * m2.m[2][1] + m1.m[2][3] * m2.m[3][1],
		m1.m[2][0] * m2.m[0][2] + m1.m[2][1] * m2.m[1][2] + m1.m[2][2] * m2.m[2][2] + m1.m[2][3] * m2.m[3][2],
		m1.m[2][0] * m2.m[0][3] + m1.m[2][1] * m2.m[1][3] + m1.m[2][2] * m2.m[2][3] + m1.m[2][3] * m2.m[3][3],

		m1.m[3][0] * m2.m[0][0] + m1.m[3][1] * m2.m[1][0] + m1.m[3][2] * m2.m[2][0] + m1.m[3][3] * m2.m[3][0],
		m1.m[3][0] * m2.m[0][1] + m1.m[3][1] * m2.m[1][1] + m1.m[3][2] * m2.m[2][1] + m1.m[3][3] * m2.m[3][1],
		m1.m[3][0] * m2.m[0][2] + m1.m[3][1] * m2.m[1][2] + m1.m[3][2] * m2.m[2][2] + m1.m[3][3] * m2.m[3][2],
		m1.m[3][0] * m2.m[0][3] + m1.m[3][1] * m2.m[1][3] + m1.m[3][2] * m2.m[2][3] + m1.m[3][3] * m2.m[3][3] };
}
inline Matrix4x4& operator*=(Matrix4x4& m1, const Matrix4x4& m2) {
	return m1 = m1 * m2;
}





/// -------------------------------------------------------------------------
/// クォータニオン
/// -------------------------------------------------------------------------
inline Quaternion operator+(const Quaternion& q) {
	return { +q.x, +q.y, +q.z, +q.w };
}
inline Quaternion operator-(const Quaternion& q) {
	return {  -q.x, -q.y, -q.z,-q.w };
}

// 加算
inline Quaternion operator+(const Quaternion& q, const float& n) {
	return { q.x + n, q.y + n, q.z + n ,q.w + n };
}
inline Quaternion operator+(const float& n, const Quaternion& q) {
	return { q.x + n, q.y + n, q.z + n, q.w + n };
}
inline Quaternion operator+(const Quaternion& q1, const Quaternion& q2) {
	return { q1.x + q2.x, q1.y + q2.y, q1.z + q2.z, q1.w + q2.w};
}
inline Quaternion& operator+=(Quaternion& q1, Quaternion& q2) {
	return q1 = q1 + q2;
}

// 減算
inline Quaternion operator-(const Quaternion& q, const float& n) {
	return { q.x - n, q.y - n, q.z - n, q.w - n };
}
inline Quaternion operator-(const float& n, const Quaternion& q) {
	return { q.x - n, q.y - n, q.z - n, q.w - n };
}
inline Quaternion operator-(const Quaternion& q1, const Quaternion& q2) {
	return { q1.x - q2.x, q1.y - q2.y, q1.z - q2.z, q1.w - q2.w };
}
inline Quaternion& operator-=(Quaternion& q1, Quaternion& q2) {
	return q1 = q1 - q2;
}

// 乗算
inline Quaternion operator*(const Quaternion& q, const float& s) {
	return { q.x * s, q.y * s, q.z * s,q.w * s };
}
inline Quaternion operator*(const float& s, const Quaternion& q) {
	return { q.x * s, q.y * s, q.z * s,q.w * s };
}
inline Quaternion operator*(const Quaternion& q1, const Quaternion& q2) {
	return {
		(q1.y * q2.z) - (q1.z * q2.y) + (q1.w * q2.x) + (q1.x * q2.w),
		(q1.z * q2.x) - (q1.x * q2.z) + (q1.w * q2.y) + (q1.y * q2.w),
		(q1.x * q2.y) - (q1.y * q2.x) + (q1.w * q2.z) + (q1.z * q2.w),
		(q1.w * q2.w) - (q1.x * q2.x) - (q1.y * q2.y) - (q1.z * q2.z)
	};
}
inline Quaternion& operator*=(Quaternion& q1, Quaternion& q2) {
	return q1 = q1 * q2;
}

// 除算
inline Quaternion operator/(const Quaternion& q, const float& s) {
	return { q.x / s, q.y / s, q.z / s, q.w / s };
}
inline Quaternion operator/(const float& s, const Quaternion& q) {
	return { q.x / s, q.y / s, q.z / s, q.w / s };
}

