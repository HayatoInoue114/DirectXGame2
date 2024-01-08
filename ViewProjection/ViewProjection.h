#pragma once
#include "../math/MT/MT.h"

class ViewProjection
{
#pragma region	ビュー行列の設定

	Vector3 scale = { 1.0f,1.0f,1.0f };
	Vector3 rotate = { 0,0,0 };
	Vector3 translate = { 0,0,-60.0f };

#pragma endregion

#pragma region 射影行列の設定

	float fov = 0.45f;
	// ビューポートのアスペクト比
	float aspectRatio = (float)16 / 9;
	// 深度限界（手前側）
	float nearZ = 0.1f;
	// 深度限界（奥側）
	float farZ = 1000.0f;

#pragma endregion

	// ビュー行列
	Matrix4x4 matView = {};
	Matrix4x4 sMatView = {};
	// 射影行列
	Matrix4x4 matProjection = {};
	Matrix4x4 sMatProjection = {};

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 行列を更新する
	/// </summary>
	void UpdateMatrix();
};

