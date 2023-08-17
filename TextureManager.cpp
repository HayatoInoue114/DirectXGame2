#include "TextureManager.h"

DirectX::ScratchImage LoadTexture(const std::string& filePath) {
	//テクスチャファイルを呼んでプログラムで扱えるようにする
	DirectX::ScratchImage image{};
	std::wstring filepathW = ConvertString(filePath);
	HRESULT hr = DirectX::LoadFromWICFile(filepathW.c_str(), DirectX::WIC_FLAGS_FORCE_SRGB, nullptr, image);
	assert(SUCCEEDED(hr));

	//ミップマップの作成
	DirectX::ScratchImage mipImage{};
	hr = DirectX::GenerateMipMaps(image.GetImages(), image.GetImageCount(), image.GetMetadata(), DirectX::TEX_FILTER_SRGB, 0, mipImage);
	assert(SUCCEEDED(hr));

	//ミップマップ付きのデータを返す
	return mipImage;
}

ID3D12Resource* TextureManager::CreateTextureResource(ID3D12Device* device, const DirectX::TexMetadata& metadata) {
	D3D12_RESOURCE_DESC resourceDesc{};
	resourceDesc.Width = UINT(metadata.width);
	resourceDesc.Height = UINT(metadata.height);
	resourceDesc.MipLevels = UINT16(metadata.mipLevels);
	resourceDesc.DepthOrArraySize = UINT16(metadata.arraySize);
	resourceDesc.Format = metadata.format;
	resourceDesc.SampleDesc.Count = 1;
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION(metadata.dimension);

	D3D12_HEAP_PROPERTIES heapProperties{};
	heapProperties.Type = D3D12_HEAP_TYPE_CUSTOM;
	heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

	ID3D12Resource* resource = nullptr;
	HRESULT hr = device->CreateCommittedResource(
		&heapProperties,
		D3D12_HEAP_FLAG_NONE,
		&resourceDesc, 
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr, 
		IID_PPV_ARGS(&resource));
	assert(SUCCEEDED(hr));
	return resource;
}

void TextureManager::UploadTextureData(ID3D12Resource* texture, const DirectX::ScratchImage& mipImages) {
	const DirectX::TexMetadata& metadata = mipImages.GetMetadata();
	for (size_t mipLevel = 0; mipLevel < metadata.mipLevels; ++mipLevel) {
		const DirectX::Image* img = mipImages.GetImage(mipLevel, 0, 0);
		HRESULT hr = texture->WriteToSubresource(
			UINT(mipLevel),
			nullptr,
			img->pixels,
			UINT(img->rowPitch),
			UINT(img->slicePitch)
		);
		assert(SUCCEEDED(hr));
	}
}

void TextureManager::LoadAndTransferTexture() {
	mipImages = LoadTexture("Resources/uvChecker.png");
	metadata = mipImages.GetMetadata();
	textureResource = CreateTextureResource(directX12_->GetDevice(), metadata);
	UploadTextureData(textureResource, mipImages);
}

void TextureManager::CreateSRV() {
	//metaDataをもとにSRVの設定
	srvDesc = {};
	srvDesc.Format = metadata.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Texture2D.MipLevels = UINT(metadata.mipLevels);

	//SRVを作成するDescriptorHeapの場所を決める
	D3D12_CPU_DESCRIPTOR_HANDLE textureSrvHandleCPU = directX12_->GetSRVDescriptorHeap()->GetCPUDescriptorHandleForHeapStart();
	D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU = directX12_->GetSRVDescriptorHeap()->GetGPUDescriptorHandleForHeapStart();

	//先頭はImGuiが使っているのでその次を使う
	textureSrvHandleCPU.ptr += directX12_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	textureSrvHandleGPU.ptr += directX12_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	//SRVの生成
	directX12_->GetDevice()->CreateShaderResourceView(textureResource, &srvDesc, textureSrvHandleCPU);
}