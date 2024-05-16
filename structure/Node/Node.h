#pragma once
#include "../../math/MyMath.h"
#include "../SkeletonData/SkeletonData.h"
struct Node
{
	QuaternionTransform transform;
	Matrix4x4 localMatrix{};
	std::string name;
	std::vector<Node> children;
};