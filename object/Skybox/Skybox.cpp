#include "Skybox.h"
#include "../../components/Light/Light.h"
#include "../../../base/GraphicsRenderer/GraphicsRenderer.h"

#pragma comment(lib,"dxcompiler.lib")

void Skybox::Initialize() {
	directX12_ = DirectX12::GetInstance();
	light_ = Light::Getinstance();

	//Transform変数を作る
	transform_.Initialize();

	CreateModel();
	CreateMaterialResource();

	CreateVertexBufferView();
	CreateResourceView();
	CreateTransformationMatrixResource();
	WriteDataToResource();

	// 右面。描画インデックスは [0, 1, 2][2, 1, 3]
	vertexData_[0].position = { 1.0f, 1.0f, 1.0f, 1.0f };
	vertexData_[1].position = { 1.0f, 1.0f, -1.0f, 1.0f };
	vertexData_[2].position = { 1.0f, -1.0f, 1.0f, 1.0f };
	vertexData_[3].position = { 1.0f, -1.0f, -1.0f, 1.0f };

	indexData_[0] = 0; indexData_[1] = 1; indexData_[2] = 2;
	indexData_[3] = 2; indexData_[4] = 1; indexData_[5] = 3;

	// 左面。描画インデックスは [4, 5, 6][6, 5, 7]
	vertexData_[4].position = { -1.0f, 1.0f, -1.0f, 1.0f };
	vertexData_[5].position = { -1.0f, 1.0f, 1.0f, 1.0f };
	vertexData_[6].position = { -1.0f, -1.0f, -1.0f, 1.0f };
	vertexData_[7].position = { -1.0f, -1.0f, 1.0f, 1.0f };

	indexData_[6] = 4; indexData_[7] = 5; indexData_[8] = 6;
	indexData_[9] = 6; indexData_[10] = 5; indexData_[11] = 7;

	// 前面。描画インデックスは [8, 9, 10][10, 9, 11]
	vertexData_[8].position = { -1.0f, 1.0f, 1.0f, 1.0f };
	vertexData_[9].position = { 1.0f, 1.0f, 1.0f, 1.0f };
	vertexData_[10].position = { -1.0f, -1.0f, 1.0f, 1.0f };
	vertexData_[11].position = { 1.0f, -1.0f, 1.0f, 1.0f };

	indexData_[12] = 8; indexData_[13] = 9; indexData_[14] = 10;
	indexData_[15] = 10; indexData_[16] = 9; indexData_[17] = 11;

	// 後面。描画インデックスは [12, 13, 14][14, 13, 15]
	vertexData_[12].position = { 1.0f, 1.0f, -1.0f, 1.0f };
	vertexData_[13].position = { -1.0f, 1.0f, -1.0f, 1.0f };
	vertexData_[14].position = { 1.0f, -1.0f, -1.0f, 1.0f };
	vertexData_[15].position = { -1.0f, -1.0f, -1.0f, 1.0f };

	indexData_[18] = 12; indexData_[19] = 13; indexData_[20] = 14;
	indexData_[21] = 14; indexData_[22] = 13; indexData_[23] = 15;

	// 上面。描画インデックスは [16, 17, 18][18, 17, 19]
	vertexData_[16].position = { -1.0f, 1.0f, -1.0f, 1.0f };
	vertexData_[17].position = { 1.0f, 1.0f, -1.0f, 1.0f };
	vertexData_[18].position = { -1.0f, 1.0f, 1.0f, 1.0f };
	vertexData_[19].position = { 1.0f, 1.0f, 1.0f, 1.0f };

	indexData_[24] = 16; indexData_[25] = 17; indexData_[26] = 18;
	indexData_[27] = 18; indexData_[28] = 17; indexData_[29] = 19;

	// 下面。描画インデックスは [20, 21, 22][22, 21, 23]
	vertexData_[20].position = { -1.0f, -1.0f, 1.0f, 1.0f };
	vertexData_[21].position = { 1.0f, -1.0f, 1.0f, 1.0f };
	vertexData_[22].position = { -1.0f, -1.0f, -1.0f, 1.0f };
	vertexData_[23].position = { 1.0f, -1.0f, -1.0f, 1.0f };

	indexData_[30] = 20; indexData_[31] = 21; indexData_[32] = 22;
	indexData_[33] = 22; indexData_[34] = 21; indexData_[35] = 23;

	SetMaterialData();
}

void Skybox::CreateResourceView()
{
	indexResource_ = directX12_->CreateBufferResource(directX12_->GetDevice().Get(), sizeof(uint32_t) * (UINT)kNumIndices_);

	indexBufferView_ = {};
	//リソースの先頭アドレスから使う
	indexBufferView_.BufferLocation = indexResource_->GetGPUVirtualAddress();
	//使用するリソースのサイズはインデックス6つ分のサイズ
	indexBufferView_.SizeInBytes = sizeof(uint32_t) * (UINT)kNumIndices_;
	//インデックスは:uint32_tとする
	indexBufferView_.Format = DXGI_FORMAT_R32_UINT;

	//インデックスリソースにデータを書き込む
	indexData_ = nullptr;
	indexResource_->Map(0, nullptr, reinterpret_cast<void**>(&indexData_));
}

void Skybox::CreateVertexBufferView() {
	vertexBufferView_ = {};
	//リソースの先頭のアドレスから使う
	vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();
	//使用するリソースのサイズは頂点３つ分のサイズ
	vertexBufferView_.SizeInBytes = UINT(sizeof(VertexData) * (UINT)kNumVertices_);
	//1頂点当たりのサイズ
	vertexBufferView_.StrideInBytes = sizeof(VertexData);
}

void Skybox::CreateMaterialResource() {
	materialResource_ = DirectX12::GetInstance()->CreateBufferResource(DirectX12::GetInstance()->GetDevice().Get(), sizeof(Material));
	materialData_ = nullptr;
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
}

void Skybox::WriteDataToResource() {
	///書き込むためのアドレスを取得
	vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));
}

void Skybox::SetMaterialData() {
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

void Skybox::CreateModel() {
	//頂点リソースを作る
	vertexResource_ = directX12_->CreateBufferResource(directX12_->GetDevice().Get(), sizeof(VertexData) * (UINT)kNumVertices_);
}

void Skybox::CreateTransformationMatrixResource() {
	//WVP用のリソースを作る。Matrix4x4　1つ分のサイズを用意する
	wvpResource_ = directX12_->CreateBufferResource(directX12_->GetDevice().Get(), sizeof(TransformationMatrix));
	//データを書き込む
	wvpData_ = nullptr;
	//書き込むためのアドレスを取得
	wvpResource_->Map(0, nullptr, reinterpret_cast<void**>(&wvpData_));
	//単位行列を書き込んでおく
	wvpData_->WVP = MakeIdentity4x4();
	wvpData_->World = MakeIdentity4x4();
}

void Skybox::CreateWVPMatrix() {
	Matrix4x4 worldMatrix = MakeAffineMatrix(transform_.scale, transform_.rotate, transform_.translate);
	Matrix4x4 worldViewProjectionMatrix{};
	if (camera_) {
		if (isParent) {
			worldMatrix = Multiply(worldMatrix, camera_->GetWorldMatrix());
		}
		const Matrix4x4& viewProjectionMatrix = camera_->GetViewProjectionMatrix();
		worldViewProjectionMatrix = Multiply(worldMatrix, viewProjectionMatrix);
	}
	else {
		worldViewProjectionMatrix = worldMatrix;
	}
	wvpData_->WVP = worldViewProjectionMatrix;
	wvpData_->World = worldMatrix;
}

void Skybox::Draw() {
	GraphicsRenderer::GetInstance()->SetRootSignatureAndPSO(GraphicsRenderer::Skybox);
	CreateWVPMatrix();
	//パラメータからUVTransform用の行列を生成する
	uvTransformMatrix_ = MakeScaleMatrix(uvTransform_.scale);
	uvTransformMatrix_ = Multiply(uvTransformMatrix_, MakeRotateZMatrix(uvTransform_.rotate.z));
	uvTransformMatrix_ = Multiply(uvTransformMatrix_, MakeTranslateMatrix(uvTransform_.translate));
	materialData_->uvTransform = uvTransformMatrix_;

	directX12_->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView_);	//VBVを設定
	//IBVを設定
	directX12_->GetCommandList()->IASetIndexBuffer(&indexBufferView_);
	//形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えておけばよい
	directX12_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	directX12_->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
	//wvp用のCBufferの場所を設定
	directX12_->GetCommandList()->SetGraphicsRootConstantBufferView(1, wvpResource_->GetGPUVirtualAddress());
	//SRV用のDescriptorTableの先頭を設定。2は:rootParameter[2]である。
	TextureManager::GetInstance()->LoadTexture("", "rostock_laage_airport_4k.dds");
	uint32_t index = TextureManager::GetInstance()->GetSrvIndex("","rostock_laage_airport_4k.dds");
	directX12_->GetCommandList()->SetGraphicsRootDescriptorTable(2, TextureManager::GetInstance()->GetGPUDescriptorHandle(index));
	//描画！(DrawCall/ドローコール)
	directX12_->GetCommandList()->DrawIndexedInstanced((UINT)kNumVertices_, 1, 0, 0, 0);
}

void Skybox::ImGuiAdjustParameter() {
	ImGui::Text("Skybox");
	ImGui::CheckboxFlags("isLighting", &materialData_->enableLighting, 1);
	ImGui::SliderFloat3("Translate", &transform_.translate.x, -5, 5);
	ImGui::SliderFloat3("Scale", &transform_.scale.x, -5, 5);
	ImGui::SliderFloat3("Rotate", &transform_.rotate.x, -5, 5);
	ImGui::Text("UVTransform");
	ImGui::DragFloat2("UVTranslate", &uvTransform_.translate.x, 0.01f, -10.0f, 10.0f);
	ImGui::DragFloat2("UVScale", &uvTransform_.scale.x, 0.01f, -10.0f, 10.0f);
	ImGui::SliderAngle("UVRotate.z", &uvTransform_.rotate.z);
}

