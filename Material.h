#pragma once
#include "Vector4.h"
#include "MT.h"

struct Material
{
	Vector4 color;
	int32_t enableLighting;
	float paring[3];
	Matrix4x4 uvTransform;
	float shininess;
};