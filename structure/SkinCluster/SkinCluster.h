#pragma once
#include "../../math/MyMath.h"
#include<array>
#include <span>
//最大4Jointの影響を受ける
const uint32_t kNumMaxInfluence = 4;

struct VertexInfluence {
	std::array<float, kNumMaxInfluence> weights;
	std::array<int32_t, kNumMaxInfluence> jointIndices;
};

struct WellForGPU {
	Matrix4x4 skeletonSpaceMatrix; //位置用
	Matrix4x4 skeletonSpaceInverseTransposeMatrix; //法線用
};

struct SkinCluster
{
	Microsoft::WRL::ComPtr<ID3D12Resource> influenceResource_;
	D3D12_VERTEX_BUFFER_VIEW influenceBufferView_;
	std::span<VertexInfluence> mappedInfluence_;
	Microsoft::WRL::ComPtr<ID3D12Resource> paletteResource_;
	std::span<WellForGPU> mappedPalette_;
	std::pair<D3D12_CPU_DESCRIPTOR_HANDLE, D3D12_GPU_DESCRIPTOR_HANDLE> paletteSrvHandle_;
};