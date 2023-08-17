#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <cassert>
#include "FormatString.h"
#include <dxgidebug.h>
#include "externals/DirectXTex/DirectXTex.h"
#include "DirectX12.h"

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"dxguid.lib")

class DirectX12;

class TextureManager
{
public:
	void LoadAndTransferTexture();
	ID3D12Resource* CreateTextureResource(ID3D12Device* device, const DirectX::TexMetadata& metadata);
	void UploadTextureData(ID3D12Resource* texture, const DirectX::ScratchImage& mipImages);
	void CreateSRV();
private:
	DirectX12* directX12_;

	DirectX::ScratchImage mipImages;

	DirectX::TexMetadata& metadata;

	ID3D12Resource* textureResource;

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
};

