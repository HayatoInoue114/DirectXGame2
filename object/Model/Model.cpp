#include "Model.h"
#include <assert.h>
#include "../../../base/GraphicsRenderer/GraphicsRenderer.h"
#include "../../manager/SrvManager/SrvManager.h"

Model* Model::CreateModelFromObj(int modelName) {
	Model* model = new Model();
	// モデルの読み込み

	model->modelData_ = ModelManager::GetInstance()->GetModelData()[modelName];

	model->Initialize();
	return model;
}

std::unique_ptr<Model> Model::CreateModelFromObjPtr(const std::string& filename) {
	std::unique_ptr<Model> model;
	model = std::make_unique<Model>();
	model->fileName_ = filename;
	// モデルの読み込み
	//model->SetTextureNum(modelName);
	ModelManager::GetInstance()->LoadModel(filename);
	model->modelData_ = ModelManager::GetInstance()->GetModel(filename);
	if (GetExtention(filename) == ".obj") {
		model->isObj = true;
	}
	if (GetExtention(filename) == ".gltf") {
		model->isObj = false;
	}

	model->Initialize();
	return model;
}

void Model::Initialize() {

	//Transform変数を作る
	worldTransform_.Initialize();

	CreateModel();
	CreateMaterialResource();

	CreateVertexBufferView();
	CreateResourceView();

	WriteDataToResource();


	SetMaterialData();
}

void Model::CreateVertexBufferView() {
	vertexBufferView_ = {};
	//リソースの先頭のアドレスから使う
	vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();
	//使用するリソースのサイズは頂点３つ分のサイズ
	vertexBufferView_.SizeInBytes = UINT(sizeof(VertexData) * modelData_.vertices.size());
	//1頂点当たりのサイズ
	vertexBufferView_.StrideInBytes = sizeof(VertexData);
}

void Model::CreateMaterialResource() {
	materialResource_ = DirectX12::GetInstance()->CreateBufferResource(DirectX12::GetInstance()->GetDevice().Get(), sizeof(Material));
	materialData_ = nullptr;
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
}

void Model::WriteDataToResource() {
	//書き込むためのアドレスを取得
	vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));
	//頂点データをリソースにコピー
	std::memcpy(vertexData_, modelData_.vertices.data(), sizeof(VertexData) * modelData_.vertices.size());
}

void Model::SetMaterialData() {
	// 色の設定
	materialData_->color = { 1.0f,1.0f,1.0f,1.0f };
	// Lightingを有効にする
	materialData_->enableLighting = true;
	//UVTransformを単位行列で初期化
	materialData_->uvTransform = MakeIdentity4x4();
	//uvTransform用の変数
	uvTransform_.scale = { 1.0f,1.0f,1.0f };
	uvTransform_.rotate = {};
	uvTransform_.translate = {};
}

void Model::CreateModel() {
	//モデル読み込み
	//modelData_ = LoadObjFile("resources", "axis");
	//頂点リソースを作る
	vertexResource_ = DirectX12::GetInstance()->CreateBufferResource(DirectX12::GetInstance()->GetDevice().Get(), sizeof(VertexData) * modelData_.vertices.size());

}

void Model::CreateResourceView()
{
	ID3D12Resource* indexResource = DirectX12::GetInstance()->CreateBufferResource(DirectX12::GetInstance()->GetDevice().Get(), sizeof(uint32_t) * modelData_.indices.size());
	
	indexBufferViewSprite_.BufferLocation = indexResource->GetGPUVirtualAddress();
	indexBufferViewSprite_.SizeInBytes = UINT(sizeof(uint32_t) * modelData_.indices.size());
	indexBufferViewSprite_.Format = DXGI_FORMAT_R32_UINT;

	uint32_t* mappedIndex = nullptr;

	indexResource->Map(0, nullptr, reinterpret_cast<void**>(&mappedIndex));
	std::memcpy(mappedIndex, modelData_.indices.data(), sizeof(uint32_t) * modelData_.indices.size());
}

void Model::LoadTexture()
{
}

void Model::Update() {
	//ImGuiAdjustParameter();
}

void Model::Draw() {
	//パラメータからUVTransform用の行列を生成する
	//GraphicsRenderer::GetInstance()->SetRootSignatureAndPSO(0);
	uvTransformMatrix_ = MakeScaleMatrix(uvTransform_.scale);
	uvTransformMatrix_ = Multiply(uvTransformMatrix_, MakeRotateZMatrix(uvTransform_.rotate.z));
	uvTransformMatrix_ = Multiply(uvTransformMatrix_, MakeTranslateMatrix(uvTransform_.translate));
	materialData_->uvTransform = uvTransformMatrix_;



	DirectX12::GetInstance()->GetCommandList()->IASetIndexBuffer(&indexBufferViewSprite_);
	//if (!isSkinning) {
	//	DirectX12::GetInstance()->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView_);	//VBVを設定
	//}
	//形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えておけばよい
	DirectX12::GetInstance()->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	DirectX12::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
	//SRV用のDescriptorTableの先頭を設定。2は:rootParameter[2]である。
	//DirectX12::GetInstance()->GetCommandList()->SetGraphicsRootDescriptorTable(2, TextureManager::GetInstance()->GetSrvHandleGPU(modelName_));
	//DirectX12::GetInstance()->GetCommandList()->SetGraphicsRootDescriptorTable(2, TextureManager::GetInstance()->GetGPUDescriptorHandle(modelName_));
	SrvManager::GetInstance()->SetGraphicsRootDescriptorTable(2, TextureManager::GetInstance()->GetSrvIndex(GetFileNameWithoutExtension(fileName_), fileName_));

	//SrvManager::GetInstance()->SetGraphicsRootDescriptorTable(4, srvIndex_);
	//描画！　（DrawCall/ドローコール)。3頂点で1つのインスタンス。
	DirectX12::GetInstance()->GetCommandList()->DrawIndexedInstanced(UINT(modelData_.indices.size()), 1, 0, 0, 0);
}

void Model::ImGuiAdjustParameter() {
	/*ImGui::Text("Model");
	ImGui::CheckboxFlags("isLighting", &materialData_->enableLighting, 1);
	ImGui::SliderFloat3("Translate", &worldTransform_.translation_.x, -5, 5);
	ImGui::SliderFloat3("Scale", &worldTransform_.scale_.x, -5, 5);
	ImGui::SliderFloat3("Rotate", &worldTransform_.rotation_.x, -5, 5);
	ImGui::Text("UVTransform");
	ImGui::DragFloat2("UVTranslate", &uvTransform_.translation_.x, 0.01f, -10.0f, 10.0f);
	ImGui::DragFloat2("UVScale", &uvTransform_.scale_.x, 0.01f, -10.0f, 10.0f);
	ImGui::SliderAngle("UVRotate.z", &uvTransform_.rotation_.z);*/
}

void Model::SetParent(const WorldTransform* parent)
{
	worldTransform_.parent_ = parent;
	isParent_ = true;
}

