#include "TextureManager.h"
#include "../../manager/SrvManager/SrvManager.h"

uint32_t TextureManager::kSRVIndexTop = 1;

void TextureManager::Initialize() {
	textureDatas.reserve(SrvManager::kMaxSRVCount);
	srvManager_ = SrvManager::GetInstance();
}

TextureManager* TextureManager::GetInstance() {
	static TextureManager instance;

	return &instance;
}

/// <summary>
/// Textureを読み込む関数(resourceはつけなくていい)
/// </summary>
/// <param name="filePath"></param>
/// <returns></returns>
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
	HRESULT hr;
	if (filePathW.ends_with(L".dds")) {// .ddsで終わっていたらddsとみなす。 より安全な方法はいくらでもあるので余裕があれば対応すると良い
		hr = DirectX::LoadFromDDSFile(filePathW.c_str(), DirectX::DDS_FLAGS_NONE, nullptr, image);
	}
	else {
		hr = DirectX::LoadFromWICFile(filePathW.c_str(), DirectX::WIC_FLAGS_FORCE_SRGB, nullptr, image);
	}

	// ミニマップの生成
	DirectX::ScratchImage mipImages{};
	if (DirectX::IsCompressed(image.GetMetadata().format)) {// 圧縮フォーマットかどうかを調べる
		mipImages = std::move(image); // 圧縮フォーマットならそのまま使うのでmoveする
	}
	else {
		hr = DirectX::GenerateMipMaps(image.GetImages(), image.GetImageCount(), image.GetMetadata(), DirectX::TEX_FILTER_SRGB, 4, mipImages);
	}

	// 追加したテクスチャデータの参照を取得
	TextureData& textureData = textureDatas["resources" + directoryPath + "/" + fileName];
	textureData.metaData = mipImages.GetMetadata();
	textureData.resource = CreateTextureResource(textureData.metaData);
	textureData.srvIndex = srvManager_->Allocate();
	textureData.srvHandleCPU = srvManager_->GetCPUDescriptorHandle(textureData.srvIndex);
	textureData.srvHandleGPU = srvManager_->GetGPUDescriptorHandle(textureData.srvIndex);

	Microsoft::WRL::ComPtr<ID3D12Resource> intermediateResource = UploadTextureData(textureData.resource, mipImages);

	//コマンドリストの内容を確定させる。全てのコマンドを積んでからCloseすること
	hr = DirectX12::GetInstance()->GetCommandList()->Close();
	assert(SUCCEEDED(hr));
	//GPUにコマンドリストの実行を行わせる
	ID3D12CommandList* commandLists[] = { DirectX12::GetInstance()->GetCommandList().Get() };
	DirectX12::GetInstance()->GetCommandQueue()->ExecuteCommandLists(1, commandLists);

	//初期値0でFenceを作る
	Microsoft::WRL::ComPtr<ID3D12Fence> fence{};
	uint64_t fenceValue{};
	//FenceのSignalを待つためのイベントを作成する
	HANDLE fenceEvent{};
	hr = DirectX12::GetInstance()->GetDevice()->CreateFence(fenceValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
	assert(SUCCEEDED(hr));
	//FenceのSignalを待つためのイベントを作成する
	fenceEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	assert(fenceEvent != nullptr);

	//Fenceの値を更新
	fenceValue++;
	//GPUがここまでたどり着いたときに、Fenceの値を指定した値に代入するようにSignalを送る
	DirectX12::GetInstance()->GetCommandQueue()->Signal(fence.Get(), fenceValue);

	//Fenceの値が出したSignal値にたどり着いているか確認する
	//GetCompletedValueの初期値はFence作成時に渡した初期値
	if (fence->GetCompletedValue() < fenceValue) {
		//指定したSignalにたどり着いていないので、たどり着くまで待つようにイベントを設定する
		fence->SetEventOnCompletion(fenceValue, fenceEvent);
		//イベントを待つ
		WaitForSingleObject(fenceEvent, INFINITE);
	}

	//次のフレーム用のコマンドリストを準備
	hr = DirectX12::GetInstance()->GetCommandAllocator()->Reset();
	assert(SUCCEEDED(hr));
	hr = DirectX12::GetInstance()->GetCommandList()->Reset(DirectX12::GetInstance()->GetCommandAllocator().Get(), nullptr);
	assert(SUCCEEDED(hr));

	srvManager_->CreateSRVforTexture2D(textureData.srvIndex, textureData.resource.Get(), textureData);
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
	Microsoft::WRL::ComPtr<ID3D12Resource> intermediateResource = DirectX12::GetInstance()->CreateBufferResource(DirectX12::GetInstance()->GetDevice().Get(), intermediateSize);
	UpdateSubresources(DirectX12::GetInstance()->GetCommandList().Get(), texture.Get(), intermediateResource.Get(), 0, 0, UINT(subresources.size()), subresources.data());
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
	TextureData& textureData = textureDatas["resources/" + filePath];
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