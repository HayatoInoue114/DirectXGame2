#pragma once
#include "../../math/MyMath.h"

struct ParticleData
{
	TransformS transform;
	Vector3 velocity;
	Vector4 color;
	float lifeTime;
	float currentTime;
};