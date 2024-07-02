#pragma once
#include "../../math/MT/MT.h"
#include "../../math/Vector4.h"

struct DirectionalLight
{
	Vector4 color; //!< ライトの色
	Vector3 direction; //!< ライトの向き
	float intensity; //!< 輝度
};
