#pragma once
#include "../MT/MT.h"

struct TransformS
{
	// ローカルスケール
	Vector3 scale = { 1, 1, 1 };
	// X,Y,Z軸回りのローカル回転角
	Vector3 rotate = { 0, 0, 0 };
	// ローカル座標
	Vector3 translate = { 0, 0, 0 };

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
};

