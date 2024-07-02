#include "Light.h"
#include <assert.h>
#include "../../base/DirectX12/DirectX12.h"

Light* Light::Getinstance() {
	static Light instance;

	return &instance;
}

ID3D12Resource* Light::CreateBufferResource(ID3D12Device* device, size_t sizeInBytes) {
	HRESULT hr;
	//頂点リソース用のヒープの設定
	D3D12_HEAP_PROPERTIES uploadHeapProperties = {};
	uploadHeapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;//UploadHeapを使う
	//リソースの設定
	D3D12_RESOURCE_DESC resourceDesc = {};
	//バッファリソース。テクスチャの場合はまた別の設定をする
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resourceDesc.Width = sizeInBytes;//リソースのサイズ。
	//バッファの場合はこれらは1にする決まり
	resourceDesc.Height = 1;
	resourceDesc.DepthOrArraySize = 1;
	resourceDesc.MipLevels = 1;
	resourceDesc.SampleDesc.Count = 1;
	//バッファの場合はこれにする決まり
	resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	//実際にリソースを作る
	ID3D12Resource* Resource = nullptr;
	hr = device->CreateCommittedResource(&uploadHeapProperties, D3D12_HEAP_FLAG_NONE,
		&resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&Resource));
	assert(SUCCEEDED(hr));

	return Resource;
}

void Light::CreateDirectionalResource() {
	directionalLightResource_ = CreateBufferResource(DirectX12::GetInstance()->GetDevice().Get(), sizeof(DirectionalLight));
	// マテリアルにデータを書き込む
	directionalLightData_ = nullptr;
	// 書き込むためのアドレスを取得
	directionalLightResource_->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightData_));
}

void Light::Initialize() {
	CreateDirectionalResource();

	// Lightingのデフォ値
	directionalLightData_->color = { 1.0f, 1.0f, 1.0f, 1.0f };
	directionalLightData_->direction = { 0.0f, 0.0f, 1.0f };
	directionalLightData_->intensity = 1.0f;
}

void Light::AdjustParameter() {
	ImGui::SliderFloat3("Lighting.direction", &directionalLightData_->direction.x, -1, 1);
	ImGui::ColorEdit3("Lighting.color", &directionalLightData_->color.x);
	ImGui::SliderFloat("Lighting.intensity", &directionalLightData_->intensity, 0, 1);
}