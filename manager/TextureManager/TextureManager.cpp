#include "TextureManager.h"

uint32_t TextureManager::kSRVIndexTop = 1;

void TextureManager::Initialize() {
	textureDatas.reserve(SrvManager::kMaxSRVCount);
	srvManager_ = SrvManager::GetInstance();
}

TextureManager* TextureManager::GetInstance() {
	static TextureManager instance;

	return &instance;
}

//void TextureManager::LoadTexture(const std::string& filePath) {
//	//読み込み済みテクスチャを検索
//	if (textureDatas.contains(filePath)) {
//		return;
//	}
//
//	//テクスチャ枚数上限チェック
//	//assert(srvManager_->IsTextureLimitReached());
//
//	assert(textureDatas.size() + kSRVIndexTop < DirectX12::kMaxSRVCount);
//
//	DirectX::ScratchImage image{};
//	std::wstring filePathW = ConvertString(filePath);
//	//HRESULT hr = DirectX::LoadFromWICFile(filePathW.c_str(), DirectX::WIC_FLAGS_FORCE_SRGB, nullptr, image);
//	HRESULT hr = DirectX::LoadFromWICFile(filePathW.c_str(), DirectX::WIC_FLAGS_NONE, nullptr, image);
//	assert(SUCCEEDED(hr));
//
//	DirectX::ScratchImage mipImages{};
//	hr = DirectX::GenerateMipMaps(image.GetImages(), image.GetImageCount(), image.GetMetadata(), DirectX::TEX_FILTER_SRGB, 0, mipImages);
//	assert(SUCCEEDED(hr));
//
//	//テクスチャデータを追加して書き込む
//	TextureData& textureData = textureDatas[filePath];
//
//	//SRV確保
//	textureData.srvIndex = srvManager_->Allocate();
//	textureData.metaData = mipImages.GetMetadata();
//	textureData.resource = CreateTextureResource(textureData.metaData);
//
//	for (size_t mipLevel = 0; mipLevel < textureData.metaData.mipLevels; ++mipLevel) {
//		const DirectX::Image* img = mipImages.GetImage(mipLevel, 0, 0);
//		hr = textureData.resource->WriteToSubresource(
//			UINT(mipLevel),
//			nullptr,
//			img->pixels,
//			UINT(img->rowPitch),
//			UINT(img->slicePitch)
//		);
//		assert(SUCCEEDED(hr));
//	}
//
//	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
//	srvDesc.Format = textureData.metaData.format;
//	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
//	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
//	srvDesc.Texture2D.MipLevels = UINT(textureData.metaData.mipLevels);
//	DirectX12::GetInstance()->GetDevice()->CreateShaderResourceView(textureData.resource.Get(), &srvDesc, textureData.srvHandleCPU);
//}

void TextureManager::LoadTexture(const std::string& directoryPath, const std::string& fileName) {
	// 読み込み済みテクスチャを検索
	if (textureDatas.contains("resources" + directoryPath + "/" + fileName)) {
		return;
	}
	// 何も読み込んでいなかったら終了
	if (fileName.size() == 0) {
		return;
	}
	// テクスチャ枚数の上限チェック
	//assert(srvManager_->GetIsLimit());

	// テクスチャファイルを読んでプログラムで扱えるようにする
	DirectX::ScratchImage image{};
	std::wstring filePathW = ConvertString("resources" + directoryPath + "/" + fileName);
	HRESULT hr = DirectX::LoadFromWICFile(filePathW.c_str(), DirectX::WIC_FLAGS_FORCE_SRGB, nullptr, image);
	assert(SUCCEEDED(hr));

	// ミニマップの生成
	DirectX::ScratchImage mipImages{};
	hr = DirectX::GenerateMipMaps(image.GetImages(), image.GetImageCount(), image.GetMetadata(), DirectX::TEX_FILTER_SRGB, 0, mipImages);
	assert(SUCCEEDED(hr));

	// 追加したテクスチャデータの参照を取得
	TextureData& textureData = textureDatas["resources" + directoryPath + "/" + fileName];
	textureData.metaData = mipImages.GetMetadata();
	textureData.resource = CreateTextureResource(textureData.metaData);
	textureData.srvIndex = srvManager_->Allocate();
	textureData.srvHandleCPU = srvManager_->GetCPUDescriptorHandle(textureData.srvIndex);
	textureData.srvHandleGPU = srvManager_->GetGPUDescriptorHandle(textureData.srvIndex);
	srvManager_->CreateSRVforTexture2D(textureData.srvIndex, textureData.resource.Get(), textureData.metaData.format, (UINT)textureData.metaData.mipLevels);

	for (size_t mipLevel = 0; mipLevel < textureData.metaData.mipLevels; ++mipLevel) {
		const DirectX::Image* img = mipImages.GetImage(mipLevel, 0, 0);
		hr = textureData.resource->WriteToSubresource(
			UINT(mipLevel),
			nullptr,
			img->pixels,
			UINT(img->rowPitch),
			UINT(img->slicePitch)
		);
		assert(SUCCEEDED(hr));
	}
}

Microsoft::WRL::ComPtr<ID3D12Resource> TextureManager::CreateTextureResource(const DirectX::TexMetadata& metadata) {
	//metadataを基にResourceの設定
	D3D12_RESOURCE_DESC resourceDesc{};
	resourceDesc.Width = UINT(metadata.width);
	resourceDesc.Height = UINT(metadata.height);
	resourceDesc.MipLevels = UINT16(metadata.mipLevels);
	resourceDesc.DepthOrArraySize = UINT16(metadata.arraySize);
	resourceDesc.Format = metadata.format;
	resourceDesc.SampleDesc.Count = 1;
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION(metadata.dimension);
	//利用するHeapの設定
	D3D12_HEAP_PROPERTIES heapProperties{};
	heapProperties.Type = D3D12_HEAP_TYPE_CUSTOM;
	heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
	//Resourceを生成する
	ID3D12Resource* resource = nullptr;
	HRESULT hr = DirectX12::GetInstance()->GetDevice()->CreateCommittedResource(
		&heapProperties,
		D3D12_HEAP_FLAG_NONE,
		&resourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&resource));
	assert(SUCCEEDED(hr));

	return resource;
}

D3D12_CPU_DESCRIPTOR_HANDLE TextureManager::GetCPUDescriptorHandle(uint32_t index) {
	D3D12_CPU_DESCRIPTOR_HANDLE handleCPU = srvManager_->GetCPUDescriptorHandle(index);
	return handleCPU;
}

D3D12_GPU_DESCRIPTOR_HANDLE TextureManager::GetGPUDescriptorHandle(uint32_t index) {
	D3D12_GPU_DESCRIPTOR_HANDLE handleGPU = srvManager_->GetGPUDescriptorHandle(index);
	return handleGPU;
}

uint32_t TextureManager::GetTextureIndexByFilePath(const std::string& directoryPath, const std::string& fileName) {
	// 読み込み済みテクスチャデータを検索
	if (textureDatas.contains("resources" + directoryPath + "/" + fileName)) {
		TextureData& textureData = textureDatas["resources" + directoryPath + "/" + fileName];
		return textureData.srvIndex;
	}
	assert(0);
	return 0;
}

const DirectX::TexMetadata& TextureManager::GetMetaData(const std::string& filePath) {
	// ファイルパスに関連付けられたテクスチャのメタデータを検索
	for (const auto& textureData : textureDatas) {
		if (textureData.first == filePath) {
			return textureData.second.metaData;
		}
	}

	throw std::runtime_error("Texture metadata not found for the specified file path: " + filePath);
}

uint32_t TextureManager::GetSrvIndex(const std::string& filePath) {
	TextureData& textureData = textureDatas[filePath];
	// 何も書いてないならデフォルトテクスチャの番号を返す
	if (filePath.size() == 0) {
		textureData = textureDatas["resources/black.png"];
	}
	return textureData.srvIndex;
}

uint32_t TextureManager::GetSrvIndex(const std::string& directoryPath, const std::string& fileName) {
	TextureData& textureData = textureDatas["resources" + directoryPath + "/" + fileName];
	// 何も書いてないならデフォルトテクスチャの番号を返す
	if (fileName.size() == 0) {
		textureData = textureDatas["resources/black.png"];
	}
	return textureData.srvIndex;
}

D3D12_GPU_DESCRIPTOR_HANDLE TextureManager::GetSrvHandleGPU(const std::string& filePath) {
	// ファイルパスに関連付けられたテクスチャのGPUディスクリプタハンドルを検索
	for (const auto& textureData : textureDatas) {
		if (textureData.first == filePath) {
			return textureData.second.srvHandleGPU;
		}
	}

	// テクスチャが見つからない場合はデフォルトのGPUディスクリプタハンドルを返す
	return D3D12_GPU_DESCRIPTOR_HANDLE{ 0 };
}