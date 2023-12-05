#pragma once
#include "../../math/Vector4.h"
#include "../../math/MT/MT.h"

struct Material
{
	Vector4 color;
	int32_t enableLighting;
	float padding[3];
	Matrix4x4 uvTransform;
};