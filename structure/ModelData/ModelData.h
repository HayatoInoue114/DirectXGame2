#pragma once
#include "../VertexData/VertexData.h"
#include <vector>
#include "../MaterialData/MaterialData.h"

struct ModelData
{
	std::vector<VertexData> vertices;
	MaterialData material;
};
