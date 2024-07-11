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

	Microsoft::WRL::ComPtr<ID3D12Resource> intermediateResource = UploadTextureData(textureData.resource, mipImages).Get();

	/*for (size_t mipLevel = 0; mipLevel < textureData.metaData.mipLevels; ++mipLevel) {
		const DirectX::Image* img = mipImages.GetImage(mipLevel, 0, 0);
		hr = textureData.resource->WriteToSubresource(
			UINT(mipLevel),
			nullptr,
			img->pixels,
			UINT(img->rowPitch),
			UINT(img->slicePitch)
		);
		assert(SUCCEEDED(hr));
	}*/
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
	heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;
	/*heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;*/
	//Resourceを生成する
	ID3D12Resource* resource = nullptr;
	HRESULT hr;
	hr = DirectX12::GetInstance()->GetDevice()->CreateCommittedResource(
		&heapProperties, // Heapの設定
		D3D12_HEAP_FLAG_NONE, // Heapの特殊な設定。特になし。
		&resourceDesc, // Resourceの設定
		D3D12_RESOURCE_STATE_COPY_DEST, // データ転送される設定
		nullptr, // Clear最適値。使わないのでnullptr
		IID_PPV_ARGS(&resource)); // 作成するResourceポインタへのポインタ
	assert(SUCCEEDED(hr));

	return resource;
}

[[nodiscard]]
Microsoft::WRL::ComPtr<ID3D12Resource> TextureManager::UploadTextureData(const Microsoft::WRL::ComPtr<ID3D12Resource>& texture, const DirectX::ScratchImage& mipImages) {
	std::vector<D3D12_SUBRESOURCE_DATA> subresources;
	DirectX::PrepareUpload(DirectX12::GetInstance()->GetDevice().Get(), mipImages.GetImages(), mipImages.GetImageCount(), mipImages.GetMetadata(), subresources);
	uint64_t intermediateSize = GetRequiredIntermediateSize(texture.Get(), 0, UINT(subresources.size()));
	ID3D12Resource* intermediateResource = DirectX12::GetInstance()->CreateBufferResource(DirectX12::GetInstance()->GetDevice().Get(), intermediateSize);
	UpdateSubresources(DirectX12::GetInstance()->GetCommandList().Get(), texture.Get(), intermediateResource, 0, 0, UINT(subresources.size()), subresources.data());
	//Textureへの転送後は利用できるようD3D12＿RESOURCE＿STATE_COPY_DESTからD3D12＿RESOURCE_STATE_GENERIC_READへResourceStateを変更する
	D3D12_RESOURCE_BARRIER barrier{};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = texture.Get();
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_GENERIC_READ;
	DirectX12::GetInstance()->GetCommandList()->ResourceBarrier(1, &barrier);
	return intermediateResource;

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