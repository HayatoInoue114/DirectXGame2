#pragma once
#include "../../math/MyMath.h"
struct Node
{
	Matrix4x4 localMatrix{};
	std::string name;
	std::vector<Node> children;
};