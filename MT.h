#pragma once
#include<math.h>
#include<cmath>

/// <summary>
/// 4x4行列
/// </summary>
struct Matrix4x4 {
	float m[4][4];
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
struct Transform
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

//逆行列
Matrix4x4 Inverse(const Matrix4x4& m);