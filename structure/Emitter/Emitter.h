#pragma once
#include "../../math/MyMath.h"

struct Emitter {
	TransformS transform;
	uint32_t count;
	float frequency;
	float frequencyTime;
};