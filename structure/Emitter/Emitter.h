#pragma once
#include "../../math/MyMath.h"

struct Emitter {
	TransformS transform;
	uint32_t count; //!< 発生数
	float frequency; //!< 発生頻度
	float frequencyTime; //!< 頻度用時刻
};