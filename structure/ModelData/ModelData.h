#pragma once
#include "../VertexData/VertexData.h"
#include <vector>
#include "../MaterialData/MaterialData.h"
#include "../Node/Node.h"
#include <map>

struct VertexWeightData
{
	float weight;
	uint32_t vertexIndex;
};

struct JointWeightData
{
	Matrix4x4 inverseBindPoseMatrix;
	std::vector<VertexWeightData> vertexWeights;
};

struct ModelData
{
	std::map<std::string, JointWeightData> skinClusterData;
	std::vector<VertexData> vertices;
	std::vector<uint32_t> indices;
	MaterialData material;
	Node rootNode;
};
