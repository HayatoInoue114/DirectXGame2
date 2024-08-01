#include "SrvManager.h"
#include <iostream>

SrvManager* SrvManager::GetInstance()
{
	static SrvManager instance;
	return &instance;
}

void SrvManager::Init()
{
	this->directX12 = DirectX12::GetInstance();

	//ディスクリプタヒープの生成
	descriptorHeap = directX12->CreateDescriptorHeap(directX12->GetDevice().Get(), D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, kMaxSRVCount, true);
	assert(descriptorHeap);
	//ディスクリプタ1個分のサイズを取得して記録
	descriptorSize = directX12->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	assert(descriptorSize > 0);
}

void SrvManager::PreDraw()
{
	//描画用のDesscriptorHeapの設定
	ID3D12DescriptorHeap* descriptorHeaps[] = { descriptorHeap.Get() };
	directX12->GetCommandList()->SetDescriptorHeaps(1, descriptorHeaps);
	assert(descriptorHeap);
}

uint32_t SrvManager::Allocate()
{
	// return する番号を一旦記録しておく
	int index = useIndex;
	// 次回のために番号を1進める
	useIndex++;
	// 上で記録した番号をreturn
	return index;
}

D3D12_CPU_DESCRIPTOR_HANDLE SrvManager::GetCPUDescriptorHandle(uint32_t index)
{
	D3D12_CPU_DESCRIPTOR_HANDLE handleCPU = descriptorHeap->GetCPUDescriptorHandleForHeapStart();
	handleCPU.ptr += (static_cast<unsigned long long>(descriptorSize) * index);
	return handleCPU;
}

D3D12_GPU_DESCRIPTOR_HANDLE SrvManager::GetGPUDescriptorHandle(uint32_t index)
{
	D3D12_GPU_DESCRIPTOR_HANDLE handleGPU = descriptorHeap->GetGPUDescriptorHandleForHeapStart();
	handleGPU.ptr += (static_cast<unsigned long long>(descriptorSize) * index);
	return handleGPU;
}

void SrvManager::SetGraphicsRootDescriptorTable(UINT RootParameterIndex, uint32_t srvIndex)
{
	directX12->GetCommandList()->SetGraphicsRootDescriptorTable(RootParameterIndex, GetGPUDescriptorHandle(srvIndex));
}

void SrvManager::CreateSRVforTexture2D(uint32_t srvIndex, ID3D12Resource* pResource, TextureData& textureData)
{
	//metaDataSRV
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = textureData.metaData.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	// ↑までは一緒。 metadataから cubemapかどうかを取得できるので利用して分岐
	if (textureData.metaData.IsCubemap()) {
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBE;
		srvDesc.TextureCube.MostDetailedMip = 0; // union TextureCubelɔth Texture2d%2
		srvDesc.TextureCube.MipLevels = UINT_MAX;
		srvDesc.TextureCube.ResourceMinLODClamp = 0.0f;
	}
	else {
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = (UINT)textureData.metaData.mipLevels;
	}

	directX12->GetDevice()->CreateShaderResourceView(pResource, &srvDesc, GetCPUDescriptorHandle(srvIndex));
}

void SrvManager::CreateSRVforStructuredBuffer(uint32_t srvIndex, ID3D12Resource* pResource, UINT numElements, UINT structureByteStride)
{
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_UNKNOWN; // 構造化バッファーのためフォーマットは UNKNOWN に設定
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER; // ビューの次元を BUFFER に設定
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Buffer.FirstElement = 0;
	srvDesc.Buffer.NumElements = numElements;
	srvDesc.Buffer.StructureByteStride = structureByteStride;
	srvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;

	directX12->GetDevice()->CreateShaderResourceView(pResource, &srvDesc, GetCPUDescriptorHandle(srvIndex));
}
