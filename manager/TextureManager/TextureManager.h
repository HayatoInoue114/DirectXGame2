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
#include "../../manager/SrvManager/SrvManager.h"
#include "../../base/DirectX12/DirectX12.h"
#include <unordered_map>

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
	TITLESPRITE,
	BLACK,
	CIRCLE,

	TEXTURENUM
};

struct TextureData
{
	DirectX::TexMetadata metaData;
	Microsoft::WRL::ComPtr<ID3D12Resource> resource;
	uint32_t srvIndex;
	D3D12_CPU_DESCRIPTOR_HANDLE srvHandleCPU;
	D3D12_GPU_DESCRIPTOR_HANDLE srvHandleGPU;
};

class TextureManager
{
public:
	static TextureManager* GetInstance();

	void Initialize();
	void LoadTexture(const std::string& filePath);
	void LoadTexture(const std::string& directoryPath, const std::string& fileName);
	Microsoft::WRL::ComPtr<ID3D12Resource> CreateTextureResource(const DirectX::TexMetadata& matadata);
	uint32_t GetTextureIndexByFilePath(const std::string& directoryPath, const std::string& fileName);
	D3D12_GPU_DESCRIPTOR_HANDLE GetSrvHandleGPU(uint32_t textureIndex);

	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(uint32_t index);
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(uint32_t index);

	// メタデータの取得
	const DirectX::TexMetadata& GetMetaData(const std::string& filePath);
	// SRVインデックスの取得
	uint32_t GetSrvIndex(const std::string& filePath);
	uint32_t GetSrvIndex(const std::string& directoryPath, const std::string& fileName);
	// GPUハンドルの取得
	D3D12_GPU_DESCRIPTOR_HANDLE GetSrvHandleGPU(const std::string& filePath);

	Microsoft::WRL::ComPtr<ID3D12Resource> UploadTextureData(const Microsoft::WRL::ComPtr<ID3D12Resource>& texture, const DirectX::ScratchImage& mipImages);
private:
	SrvManager* srvManager_;

	static uint32_t kSRVIndexTop;

	std::unordered_map<std::string, TextureData> textureDatas;

	//static const size_t kMaxSRVCount = 2056;
	//std::array<Microsoft::WRL::ComPtr<ID3D12Resource>, kMaxSRVCount> textureBuffers_;

	TextureManager() = default;
	~TextureManager() = default;
	TextureManager(TextureManager&) = delete;
	TextureManager& operator=(TextureManager&) = delete;

};

