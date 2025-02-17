#include "Sphere.h"
#include "../../components/Light/Light.h"
#include "../../../base/GraphicsRenderer/GraphicsRenderer.h"

#pragma comment(lib,"dxcompiler.lib")

void Sphere::Initialize() {
	directX12_ = DirectX12::GetInstance();
	light_ = Light::Getinstance();

	//Transform変数を作る
	transform_.Initialize();

	CreateVertexResource();
	CreateMaterialResource();
	CreateVertexBufferView();
	CreateTransformationMatrixResource();
	WriteDataToResource();

	//軽度分割数1つ分の角度
	const float kLonEvery = (2 * M_PI) / kSubdivision;
	//軽度分割1つ分の角度
	const float kLatEvery = (M_PI) / kSubdivision;

	// 緯度の方向に分割
	for (uint32_t latIndex = 0; latIndex < kSubdivision; ++latIndex) {
		float lat = float(-M_PI) / 2.0f + kLatEvery * latIndex;
		// 経度の方向に分割しながら線を描く
		for (uint32_t lonIndex = 0; lonIndex < kSubdivision; ++lonIndex) {
			uint32_t start = (latIndex * kSubdivision + lonIndex) * 6;
			float lon = lonIndex * kLonEvery;

			// 基準点a
			vertexData_[start].position.x = cosf(lat) * cosf(lon);
			vertexData_[start].position.y = sinf(lat);
			vertexData_[start].position.z = cosf(lat) * sinf(lon);
			vertexData_[start].position.w = 1.0f;
			vertexData_[start].texcoord = { float(lonIndex) / float(kSubdivision) ,1.0f - float(latIndex) / float(kSubdivision) };
			vertexData_[start].normal.x = vertexData_[start].position.x;
			vertexData_[start].normal.y = vertexData_[start].position.y;
			vertexData_[start].normal.z = vertexData_[start].position.z;
			// b
			vertexData_[start + 1].position.x = cosf(lat + kLatEvery) * cosf(lon);
			vertexData_[start + 1].position.y = sinf(lat + kLatEvery);
			vertexData_[start + 1].position.z = cosf(lat + kLatEvery) * sinf(lon);
			vertexData_[start + 1].position.w = 1.0f;
			vertexData_[start + 1].texcoord = { vertexData_[start].texcoord.x,vertexData_[start].texcoord.y - (1.0f / kSubdivision) };
			vertexData_[start + 1].normal.x = vertexData_[start + 1].position.x;
			vertexData_[start + 1].normal.y = vertexData_[start + 1].position.y;
			vertexData_[start + 1].normal.z = vertexData_[start + 1].position.z;
			// c
			vertexData_[start + 2].position.x = cosf(lat) * cosf(lon + kLonEvery);
			vertexData_[start + 2].position.y = sinf(lat);
			vertexData_[start + 2].position.z = cosf(lat) * sinf(lon + kLonEvery);
			vertexData_[start + 2].position.w = 1.0f;
			vertexData_[start + 2].texcoord = { vertexData_[start].texcoord.x + (1.0f / (float)kSubdivision),vertexData_[start].texcoord.y };
			vertexData_[start + 2].normal.x = vertexData_[start + 2].position.x;
			vertexData_[start + 2].normal.y = vertexData_[start + 2].position.y;
			vertexData_[start + 2].normal.z = vertexData_[start + 2].position.z;


			// b
			vertexData_[start + 3].position.x = cosf(lat + kLatEvery) * cosf(lon);
			vertexData_[start + 3].position.y = sinf(lat + kLatEvery);
			vertexData_[start + 3].position.z = cosf(lat + kLatEvery) * sinf(lon);
			vertexData_[start + 3].position.w = 1.0f;
			vertexData_[start + 3].texcoord = { vertexData_[start].texcoord.x,vertexData_[start].texcoord.y - (1.0f / (float)kSubdivision) };
			vertexData_[start + 3].normal.x = vertexData_[start + 3].position.x;
			vertexData_[start + 3].normal.y = vertexData_[start + 3].position.y;
			vertexData_[start + 3].normal.z = vertexData_[start + 3].position.z;
			// d
			vertexData_[start + 4].position.x = cosf(lat + kLatEvery) * cosf(lon + kLonEvery);
			vertexData_[start + 4].position.y = sinf(lat + kLatEvery);
			vertexData_[start + 4].position.z = cosf(lat + kLatEvery) * sinf(lon + kLonEvery);
			vertexData_[start + 4].position.w = 1.0f;
			vertexData_[start + 4].texcoord = { vertexData_[start].texcoord.x + (1.0f / (float)kSubdivision),vertexData_[start].texcoord.y - (1.0f / (float)kSubdivision) };
			vertexData_[start + 4].normal.x = vertexData_[start + 4].position.x;
			vertexData_[start + 4].normal.y = vertexData_[start + 4].position.y;
			vertexData_[start + 4].normal.z = vertexData_[start + 4].position.z;
			// c
			vertexData_[start + 5].position.x = cosf(lat) * cosf(lon + kLonEvery);
			vertexData_[start + 5].position.y = sinf(lat);
			vertexData_[start + 5].position.z = cosf(lat) * sinf(lon + kLonEvery);
			vertexData_[start + 5].position.w = 1.0f;
			vertexData_[start + 5].texcoord = { vertexData_[start].texcoord.x + (1.0f / (float)kSubdivision),vertexData_[start].texcoord.y };
			vertexData_[start + 5].normal.x = vertexData_[start + 5].position.x;
			vertexData_[start + 5].normal.y = vertexData_[start + 5].position.y;
			vertexData_[start + 5].normal.z = vertexData_[start + 5].position.z;
		}
	}
	SetMaterialData();
}

void Sphere::CreateVertexResource() {
	vertexResource_ = directX12_->CreateBufferResource(directX12_->GetDevice().Get(), sizeof(VertexData) * vertexIndex_);
}

void Sphere::CreateVertexBufferView() {
	vertexBufferView = {};
	//リソースの先頭のアドレスから使う
	vertexBufferView.BufferLocation = vertexResource_->GetGPUVirtualAddress();
	//使用するリソースのサイズは頂点３つ分のサイズ
	vertexBufferView.SizeInBytes = sizeof(VertexData) * vertexIndex_;
	//1頂点当たりのサイズ
	vertexBufferView.StrideInBytes = sizeof(VertexData);
}

void Sphere::CreateMaterialResource() {
	materialResource_ = directX12_->CreateBufferResource(directX12_->GetDevice().Get(), sizeof(Material));
	materialData_ = nullptr;
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
}

void Sphere::CreateTransformationMatrixResource() {
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

void Sphere::WriteDataToResource() {
	//書き込むためのアドレスを取得
	vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));
}

void Sphere::CreateWVPMatrix() {
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

void Sphere::SetMaterialData() {
	// 色の設定
	materialData_->color = { 1.0f,1.0f,1.0f,1.0f };
	// Lightingを有効にする
	materialData_->enableLighting = true;
	//UVTransformを単位行列で初期化
	materialData_->uvTransform = MakeIdentity4x4();
	//uvTransform用の変数
	uvTransform_.Initialize();

}

void Sphere::Update(WorldTransform& transform, Vector4& color) {
	transform_ = transform;
	CreateWVPMatrix();
	//色の指定
	materialData_->color = color;
	ImGuiAdjustParameter();
}

void Sphere::Draw() {
	GraphicsRenderer::GetInstance()->SetRootSignatureAndPSO(0);
	CreateWVPMatrix();
	//パラメータからUVTransform用の行列を生成する
	uvTransformMatrix_ = MakeScaleMatrix(uvTransform_.scale);
	uvTransformMatrix_ = Multiply(uvTransformMatrix_, MakeRotateZMatrix(uvTransform_.rotate.z));
	uvTransformMatrix_ = Multiply(uvTransformMatrix_, MakeTranslateMatrix(uvTransform_.translate));
	materialData_->uvTransform = uvTransformMatrix_;

	directX12_->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView);	//VBVを設定
	//形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えておけばよい
	directX12_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	directX12_->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
	//wvp用のCBufferの場所を設定
	directX12_->GetCommandList()->SetGraphicsRootConstantBufferView(1, wvpResource_->GetGPUVirtualAddress());
	//SRV用のDescriptorTableの先頭を設定。2は:rootParameter[2]である。
	directX12_->GetCommandList()->SetGraphicsRootDescriptorTable(2, TextureManager::GetInstance()->GetGPUDescriptorHandle(0));
	directX12_->GetCommandList()->SetGraphicsRootConstantBufferView(3, light_->GetDirectionalLightResource()->GetGPUVirtualAddress());

	//描画！　（DrawCall/ドローコール)。3頂点で1つのインスタンス。
	directX12_->GetCommandList()->DrawInstanced(vertexIndex_, 1, 0, 0);
}

void Sphere::ImGuiAdjustParameter() {
	ImGui::Text("Sphere");
	ImGui::CheckboxFlags("isLighting", &materialData_->enableLighting, 1);
	ImGui::Checkbox("useMonsterBall", &useMonsterBall_);
	ImGui::SliderFloat3("Translate", &transform_.translate.x, -5, 5);
	ImGui::SliderFloat3("Scale", &transform_.scale.x, -5, 5);
	ImGui::SliderFloat3("Rotate", &transform_.rotate.x, -5, 5);
	ImGui::Text("UVTransform");
	ImGui::DragFloat2("UVTranslate", &uvTransform_.translate.x, 0.01f, -10.0f, 10.0f);
	ImGui::DragFloat2("UVScale", &uvTransform_.scale.x, 0.01f, -10.0f, 10.0f);
	ImGui::SliderAngle("UVRotate.z", &uvTransform_.rotate.z);
}

