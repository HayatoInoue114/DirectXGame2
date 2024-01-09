#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <cassert>
#include "../../utility/FormatString/FormatString.h"
#include <dxgidebug.h>
#include "../../math/Vector4.h"
#include "../../manager/ImGuiManager/ImGuiManager.h"
#include "../../externals/DirectXTex/DirectXTex.h"
#include <wrl.h>
#include "../../structure/ModelData/ModelData.h"

#include "../../base/DirectX12/DirectX12.h"

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"dxguid.lib")

enum TextureName {
	UVCHECKER,
	MONSTERBALL,
	//PLAYER,
	//ENEMY,
	RETICLE,
	PLAYERBULLET,
	ENEMYBULLET,
	SKYDOMETEX,
	CLEAR,
	TITLE,

	TEXTURENUM
};
class TextureManager
{
public:
	static TextureManager* GetInstance();

	// Textureを読む
	DirectX::ScratchImage LoadTexture(const std::string& filePath);

	// DirectX12のTextureResourceを作る
	Microsoft::WRL::ComPtr<ID3D12Resource> CreateTextureResource(const DirectX::TexMetadata& metadata);

	// 中間リソースの生成
	//Microsoft::WRL::ComPtr<ID3D12Resource> CreateBufferResource(const Microsoft::WRL::ComPtr<ID3D12Device>& device, size_t sizeInBytes);

	// TextureResourceにデータを転送する
	//Microsoft::WRL::ComPtr<ID3D12Resource> UploadTextureData(const Microsoft::WRL::ComPtr<ID3D12Resource>& texture, const DirectX::ScratchImage& mipImages);
	void UploadTextureData(const Microsoft::WRL::ComPtr<ID3D12Resource>& texture, const DirectX::ScratchImage& mipImages);
	
	// textureを読んで転送する
	void LoadAndTransferTexture();

	// スプライトの初期化
	void Initialize();

	void CreateResouce();

	//void CreateSRV();

	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(const Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& descriptorHeap, uint32_t descriptorSize, uint32_t index);
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(const Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& descriptorHeap, uint32_t descriptorSize, uint32_t index);

	D3D12_GPU_DESCRIPTOR_HANDLE* GetTextureSrvHandleGPU() { return textureSrvHandleGPU_; }
private:

	static const uint32_t kMaxImages = TEXTURENUM;

	DirectX::ScratchImage mipImages_[kMaxImages]{};

	DirectX::TexMetadata metadata_[kMaxImages]{};

	uint32_t descriptorSizeSRV_[kMaxImages]{};

	D3D12_CPU_DESCRIPTOR_HANDLE textureSrvHandleCPU_[kMaxImages]{};
	D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU_[kMaxImages]{};

	Microsoft::WRL::ComPtr<ID3D12Resource> textureResource_[kMaxImages]{};
	Microsoft::WRL::ComPtr<ID3D12Resource> intermediateResource_[kMaxImages]{};

	// objデータ
	static const int32_t kMaxObjModelData = 1;
	ModelData* modelData_{};
};

