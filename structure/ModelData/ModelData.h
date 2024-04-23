#pragma once
#include "../VertexData/VertexData.h"
#include <vector>
#include "../MaterialData/MaterialData.h"
#include "../Node/Node.h"

struct ModelData
{
	std::vector<VertexData> vertices;
	MaterialData material;
	Node rootNode;
};
