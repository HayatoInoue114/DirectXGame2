#pragma once
#include "../../math/MyMath.h"

struct AccField {
	Vector3 acc;  // 加速度
	AABB area;	  // 範囲
	bool isActive;// Fieldの活性化
};