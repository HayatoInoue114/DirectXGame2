#pragma once
#include "../../base/DirectX12/DirectX12.h"



class SrvManager
{
public:
	static SrvManager* GetInstance();

	void Init();

	void PreDraw();

	uint32_t Allocate();

	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(uint32_t index);
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(uint32_t index);

	void SetGraphicsRootDescriptorTable(UINT RootParameterIndex, uint32_t srvIndex);

	// SRV生成（テクスチャ用）
	void CreateSRVforTexture2D(uint32_t srvIndex, ID3D12Resource* pResource, DXGI_FORMAT format, UINT mipLevels);
	//SRV生成（Structure Buffer用）
	void CreateSRVforStructuredBuffer(uint32_t srvIndex, ID3D12Resource* pResource, UINT numElements, UINT structureByteStride);

	//bool IsTextureLimitReached() {};

	static const uint32_t kMaxSRVCount = 512;
private:
	

private:	///	メンバ変数 ///

	DirectX12* directX12 = nullptr;
	//SRV用ディスクリプタサイズ
	uint32_t descriptorSize;
	//SRV用ディスクリプタヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeap;
	//次に使用するSRVインデックス
	uint32_t useIndex = 0;

private:	/// メンバ定数 ///

	//最大SRV数（最大テクスチャ枚数）
	static const uint32_t kMaxSRVCount;
};

