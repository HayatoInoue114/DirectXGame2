#pragma once
#include "VertexData.h"
#include <vector>
#include "MaterialData.h"

struct ModelData
{
	std::vector<VertexData> vertices;
	MaterialData material;
};
