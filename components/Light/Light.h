#pragma once
#include "../DirectionalLight/DirectionalLight.h"
#include <d3d12.h>

class DirectX12;

class Light
{
public:
	static Light* Getinstance();

	ID3D12Resource* GetDirectionalLightResource() { return directionalLightResource_; }

	

	void Initialize(DirectX12* directX12);

	void AdjustParameter();

private:
	ID3D12Resource* CreateBufferResource(ID3D12Device* device, size_t sizeInBytes);

	void CreateDirectionalResource();

private:
	DirectX12* directX12_;

	ID3D12Resource* directionalLightResource_;
	DirectionalLight* directionalLightData_;
};

