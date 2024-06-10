#pragma once
#include "../../../structure/structure.h"
#include "../../../base/DirectX12/DirectX12.h"
#include "../../../manager/TextureManager/TextureManager.h"

class SkinCluster
{
public:
	SkinClusterData Create(const Microsoft::WRL::ComPtr<ID3D12Device>& device, const Skeleton& skeleton,
		const ModelData& modelData);

	void Update(SkinClusterData& skinCluster, const Skeleton& skeleton);

private:

};

