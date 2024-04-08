#include "ResourcePeroperty.h"



/// <summary>
/// インスタンスの取得
/// </summary>
CreateResource* CreateResource::GetInstance() {
	static CreateResource instance;
	return &instance;
}



/// <summary>
/// 頂点バッファリソースを作成する
/// </summary>
ComPtr<ID3D12Resource> CreateResource::CreateBufferResource(size_t sizeInBytes) {

	ComPtr<ID3D12Resource> resultResource;

	// 頂点リソース用のヒープ設定
	D3D12_HEAP_PROPERTIES uploadHeapProperties_{};
	uploadHeapProperties_.Type = D3D12_HEAP_TYPE_UPLOAD; // UploadHeapを使う

	// バッファリソース。テクスチャの場合はまた別の設定をする
	D3D12_RESOURCE_DESC vertexResourceDesc_{};
	vertexResourceDesc_.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;

	// リソースのサイズ
	vertexResourceDesc_.Width = sizeInBytes;

	// バッファの場合はこれらは1にする決まり
	vertexResourceDesc_.Height = 1;
	vertexResourceDesc_.DepthOrArraySize = 1;
	vertexResourceDesc_.MipLevels = 1;
	vertexResourceDesc_.SampleDesc.Count = 1;

	// バッファの場合はこれにする決まり
	vertexResourceDesc_.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// 実際に頂点リソースを作る
	HRESULT hr_;
	hr_ = DirectX12::GetInstance()->GetDevice()->CreateCommittedResource(
		&uploadHeapProperties_, D3D12_HEAP_FLAG_NONE,
		&vertexResourceDesc_, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&resultResource));
	assert(SUCCEEDED(hr_));


	return resultResource;
}
void CreateResource::CreateBufferResource(size_t sizeInBytes, ComPtr<ID3D12Resource>& resource) {

	ComPtr<ID3D12Resource> resultResource;

	// 頂点リソース用のヒープ設定
	D3D12_HEAP_PROPERTIES uploadHeapProperties_{};
	uploadHeapProperties_.Type = D3D12_HEAP_TYPE_UPLOAD; // UploadHeapを使う

	// バッファリソース。テクスチャの場合はまた別の設定をする
	D3D12_RESOURCE_DESC vertexResourceDesc_{};
	vertexResourceDesc_.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;

	// リソースのサイズ
	vertexResourceDesc_.Width = sizeInBytes;

	// バッファの場合はこれらは1にする決まり
	vertexResourceDesc_.Height = 1;
	vertexResourceDesc_.DepthOrArraySize = 1;
	vertexResourceDesc_.MipLevels = 1;
	vertexResourceDesc_.SampleDesc.Count = 1;

	// バッファの場合はこれにする決まり
	vertexResourceDesc_.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// 実際に頂点リソースを作る
	HRESULT hr_;
	hr_ = DirectX12::GetInstance()->GetDevice()->CreateCommittedResource(
		&uploadHeapProperties_, D3D12_HEAP_FLAG_NONE,
		&vertexResourceDesc_, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&resource));
	assert(SUCCEEDED(hr_));
}



/// <summary>
/// 頂点バッファビューを作成する
/// </summary>
D3D12_VERTEX_BUFFER_VIEW CreateResource::CreateVertexBufferView(size_t sizeInbyte, ID3D12Resource* resource, int size) {

	D3D12_VERTEX_BUFFER_VIEW resultBufferView{};

	// 頂点バッファビューを作成する
	// リソースの先頭アドレスから使う
	resultBufferView.BufferLocation = resource->GetGPUVirtualAddress();

	// 使用するリソースのサイズは頂点3つ分のサイズ
	resultBufferView.SizeInBytes = UINT(sizeInbyte);

	// 1頂点あたりのサイズ
	resultBufferView.StrideInBytes = UINT(sizeInbyte / size);


	return resultBufferView;
}
D3D12_INDEX_BUFFER_VIEW CreateResource::CreateIndexBufferview(size_t sizeInbyte, ID3D12Resource* resource) {

	D3D12_INDEX_BUFFER_VIEW resultBufferView{};

	// リソースの先頭のアドレスから使う
	resultBufferView.BufferLocation = resource->GetGPUVirtualAddress();

	// 使用するリソースサイズはインデックス６つ分のサイズ
	resultBufferView.SizeInBytes = UINT(sizeInbyte);

	// インデックスはuint32_tとする
	resultBufferView.Format = DXGI_FORMAT_R32_UINT;

	return resultBufferView;
}
