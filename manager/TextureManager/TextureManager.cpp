#include "TextureManager.h"

TextureManager* TextureManager::GetInstance() {
	static TextureManager instance;

	return &instance;
}

void TextureManager::Initialize() {
	LoadAndTransferTexture();
}

DirectX::ScratchImage TextureManager::LoadTexture(const std::string& filePath) {
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

Microsoft::WRL::ComPtr<ID3D12Resource> TextureManager::CreateTextureResource(const DirectX::TexMetadata& metadata) {
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

void TextureManager::UploadTextureData(const Microsoft::WRL::ComPtr<ID3D12Resource>& texture, const DirectX::ScratchImage& mipImages) {
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
	//ここで.pngを読み込む
	mipImages_[UVCHECKER] = LoadTexture("resources/uvChecker.png");
	mipImages_[MONSTERBALL] = LoadTexture("resources/monsterBall.png");
	//mipImages_[PLAYER] = LoadTexture("resources/godest.png");
	//mipImages_[ENEMY] = LoadTexture("resources/title_jiki.png");
	mipImages_[RETICLE] = LoadTexture("resources/reticle.png");
	mipImages_[PLAYERBULLET] = LoadTexture("resources/ball.png");
	mipImages_[ENEMYBULLET] = LoadTexture("resources/ball.png");
	mipImages_[SKYDOMETEX] = LoadTexture("resources/colorGrid.png");
	mipImages_[CLEAR] = LoadTexture("resources/clear.png");
	mipImages_[TITLESPRITE] = LoadTexture("resources/titleSprite.png");
	mipImages_[BLACK] = LoadTexture("resources/black.png");

	//Uploadする
	for (uint32_t i = 0; i < kMaxImages; i++) {
		metadata_[i] = mipImages_[i].GetMetadata();
		textureResource_[i] = CreateTextureResource(metadata_[i]).Get();
		UploadTextureData(textureResource_[i].Get(), mipImages_[i]);
	}

	// DescriptorSizeを取得
	for (uint32_t i = 0; i < kMaxImages; i++) {
		descriptorSizeSRV_[i] = DirectX12::GetInstance()->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	}

	// metaDataをもとにSRVの設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc[kMaxImages]{};
	for (uint32_t i = 0; i < kMaxImages; i++) {
		srvDesc[i].Format = metadata_[i].format;
		srvDesc[i].Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srvDesc[i].ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srvDesc[i].Texture2D.MipLevels = UINT(metadata_[i].mipLevels);
		// SRVを作成するDescriptorHeapの場所を決める
		textureSrvHandleCPU_[i] = GetCPUDescriptorHandle(DirectX12::GetInstance()->GetSrvDescriptorHeap(), descriptorSizeSRV_[i], i + 1);
		textureSrvHandleGPU_[i] = GetGPUDescriptorHandle(DirectX12::GetInstance()->GetSrvDescriptorHeap(), descriptorSizeSRV_[i], i + 1);
		// SRVの生成
		DirectX12::GetInstance()->GetDevice()->CreateShaderResourceView(textureResource_[i].Get(), &srvDesc[i], textureSrvHandleCPU_[i]);
	}

}

void TextureManager::CreateResouce() {

}




//Microsoft::WRL::ComPtr<ID3D12Resource> TextureManager::CreateBufferResource(const Microsoft::WRL::ComPtr<ID3D12Device>& device, size_t sizeInBytes) {
//	HRESULT hr;
//	//頂点リソース用のヒープの設定
//	D3D12_HEAP_PROPERTIES uploadHeapProperties = {};
//	uploadHeapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;//UploadHeapを使う
//	//リソースの設定
//	D3D12_RESOURCE_DESC resourceDesc = {};
//	//バッファリソース。テクスチャの場合はまた別の設定をする
//	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
//	resourceDesc.Width = sizeInBytes * 3;//リソースのサイズ。今回はVector４を３頂点分
//	//バッファの場合はこれらは1にする決まり
//	resourceDesc.Height = 1;
//	resourceDesc.DepthOrArraySize = 1;
//	resourceDesc.MipLevels = 1;
//	resourceDesc.SampleDesc.Count = 1;
//	//バッファの場合はこれにする決まり
//	resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
//	//実際にリソースを作る
//	ID3D12Resource* Resource = nullptr;
//	hr = device->CreateCommittedResource(&uploadHeapProperties, D3D12_HEAP_FLAG_NONE,
//		&resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
//		IID_PPV_ARGS(&Resource));
//	assert(SUCCEEDED(hr));
//
//	return Resource;
//}

D3D12_CPU_DESCRIPTOR_HANDLE TextureManager::GetCPUDescriptorHandle(const Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& descriptorHeap, uint32_t descriptorSize, uint32_t index) {
	D3D12_CPU_DESCRIPTOR_HANDLE handleCPU = descriptorHeap->GetCPUDescriptorHandleForHeapStart();
	handleCPU.ptr += (static_cast<unsigned long long>(descriptorSize) * index);
	return handleCPU;
}

D3D12_GPU_DESCRIPTOR_HANDLE TextureManager::GetGPUDescriptorHandle(const Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& descriptorHeap, uint32_t descriptorSize, uint32_t index) {
	D3D12_GPU_DESCRIPTOR_HANDLE handleGPU = descriptorHeap->GetGPUDescriptorHandleForHeapStart();
	handleGPU.ptr += (static_cast<unsigned long long>(descriptorSize) * index);
	return handleGPU;
}