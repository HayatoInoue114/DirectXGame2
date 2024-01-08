#pragma once
#include<math.h>
#include<cmath>
#include "../Vector4.h"

#define M_PI 3.14f

struct Vector2
{
	float x;
	float y;
};

/// <summary>
/// 4x4行列
/// </summary>
struct Matrix4x4 {
	float m[4][4];
};

/// <summary>
/// 3x3行列
/// </summary>
struct Matrix3x3 {
	float m[3][3];
};

/// <summary>
/// 3次元ベクトル
/// </summary>
struct Vector3 {
	float x;
	float y;
	float z;
};

/// <summary>
/// オブジェクトの位置・姿勢情報
/// </summary>
/// <param name="Scale">スケール</param>
/// <param name="rotate">回転</param>
/// <param name="translate">座標</param>
struct Transform
{
	Vector3 scale;
	Vector3 rotate;
	Vector3 translate;
};

/// <summary>
/// オブジェクトの位置・姿勢情報
/// </summary>
/// <param name="Scale">スケール</param>
/// <param name="rotate">回転</param>
/// <param name="translate">座標</param>
struct WorldTransform
{
	Vector3 scale;
	Vector3 rotate;
	Vector3 translate;
};

/// <summary>
/// 単位行列の作成
/// </summary>
Matrix4x4 MakeIdentity4x4();

/// <summary>
/// 行列の積
/// </summary>
Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);

//X軸回転行列
Matrix4x4 MakeRotateXMatrix(float radian);
//Y軸回転行列
Matrix4x4 MakeRotateYMatrix(float radian);
//Z軸回転行列
Matrix4x4 MakeRotateZMatrix(float radian);

//3次元アフィン変換行列
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);

float InverseNum(const Matrix4x4& m, int a, int b, int c, int d, int e, int f, int g, int h);
float InverseNum2(const Matrix4x4& m, int a, int b, int c, int d, int e, int f);

// 転置行列
Matrix4x4 Transpose(const Matrix4x4& m);
//逆行列
Matrix4x4 Inverse(const Matrix4x4& m);

//1.透視投影行列
Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip);
//2.正射影行列
Matrix4x4 MakeOrthographicMatrix(float left, float right, float top, float bottom, float nearClip, float farClip);
//3.ビューポート変換行列
Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth);

Matrix4x4 MakeScaleMatrix(const Vector3& scale);

Matrix4x4 MakeTranslateMatrix(const Vector3& translate);

// 正規化
Vector3 Normalize(const Vector3& v);

//Vector3型の和
Vector3 Add(const Vector3& v1, const Vector3& v2);
Vector2 Add(const Vector2& v1, const Vector2& v2);

// 長さ(ノルム)
float Length(const Vector3& v);

// 内積
float Dot(const Vector3& v1, const Vector3& v2);

//球面線形補間
Vector3 Slerp(const Vector3& v1, const Vector3& v2, float t);

// 減算
Vector3 Subtract(const Vector3& v1, const Vector3& v2);
Vector2 Subtract(const Vector2& v1, const Vector2& v2);

// スカラー倍
Vector3 Multiply(const Vector3& v, float scalar);

// 正面へのベクトルを取得
Vector3 ForwardVector(const Vector3& rotate);

//線形補間
float Lerp(float a, float b, float t);

//線形補間
Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t);

struct AABB
{
	Vector3 min; //!< 最小点
	Vector3 max; //!< 最大点
};

//AABBの当たり判定
bool IsCollision(const AABB& aabb1, const AABB& aabb2);

//AABBを作る
AABB CreateAABB(const Vector3& scale, const Vector3& translate);

//ベクトルの除算
Vector3 Divide(const Vector3& v, float scalar);

struct LineSegment {
	Vector3 start, end;
};

struct Segment
{
	Vector3 origin; //!< 始点
	Vector3 diff; //!< 終点への差分ベクトル
};

// AABBと線分
bool IsCollision(const AABB& box, const LineSegment& line);

//bool IsCollision(const AABB& aabb, const Segment& segment);

int Min(int num1, int num2);

float Min(float num1, float num2);

int Max(int num1, int num2);

float Max(float num1, float num2);

Vector2 operator+(const Vector2& a, const Vector2& b);

Vector2 operator-(const Vector2& a, const Vector2& b);

Vector2 operator*(const Vector2& a, const Vector2& b);

Vector2 operator*(const float& a, const Vector2& b);

Vector3 operator+(const Vector3& a, const Vector3& b);

Vector3 operator+(const Vector3& a, const float& b);

Vector3 operator-(const Vector3& a, const Vector3& b);

Vector3 operator-(const Vector3& a, const float& b);

Vector3 operator*(const Vector3& a, const Vector3& b);

Vector3 operator*(const float& a, const Vector3& b);

Vector3 operator*(const Vector3& vec, const Matrix4x4& mat);