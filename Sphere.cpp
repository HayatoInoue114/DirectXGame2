#include "Sphere.h"
#include "Light.h"

void Sphere::Initialize(DirectX12* directX12, Light* light) {
	directX12_ = directX12;
	light_ = light;

	//Transform変数を作る
	transform_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };

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

	// 色の設定
	materialData_->color = { 1.0f,1.0f,1.0f,1.0f };
	// Lightingを有効にする
	materialData_->enableLighting = true;
}

void Sphere::CreateVertexResource() {
	vertexResource = directX12_->CreateBufferResource(directX12_->GetDevice(), sizeof(VertexData) * vertexIndex_);
}

void Sphere::CreateVertexBufferView() {
	vertexBufferView = {};
	//リソースの先頭のアドレスから使う
	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();
	//使用するリソースのサイズは頂点３つ分のサイズ
	vertexBufferView.SizeInBytes = sizeof(VertexData) * vertexIndex_;
	//1頂点当たりのサイズ
	vertexBufferView.StrideInBytes = sizeof(VertexData);
}

void Sphere::CreateMaterialResource() {
	materialResource_ = directX12_->CreateBufferResource(directX12_->GetDevice(), sizeof(Material));
	materialData_ = nullptr;
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
}

void Sphere::CreateTransformationMatrixResource() {
	//WVP用のリソースを作る。Matrix4x4　1つ分のサイズを用意する
	wvpResource_ = directX12_->CreateBufferResource(directX12_->GetDevice(), sizeof(TransformationMatrix));
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
	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));
}

void Sphere::CreateWVPMatrix() {
	cameraTransform_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,-10.0f,} };

	worldMatrix_ = MakeAffineMatrix(transform_.scale, transform_.rotate, transform_.translate);
	cameramatrix_ = MakeAffineMatrix(cameraTransform_.scale, cameraTransform_.rotate, cameraTransform_.translate);
	viewMatrix_ = Inverse(cameramatrix_);
	projectionMatix_ = MakePerspectiveFovMatrix(0.45f, float(kCliantWidth) / float(kCliantHeight), 0.1f, 100.0f);
	worldViewProjectionMatrix_ = Multiply(worldMatrix_, Multiply(viewMatrix_, projectionMatix_));
	wvpData_->WVP = worldViewProjectionMatrix_;
	wvpData_->World = worldMatrix_;
}

void Sphere::Release() {
	vertexResource->Release();
	materialResource_->Release();
}

void Sphere::Update(Transform& transform, Vector4& color) {
	transform_ = transform;
	CreateWVPMatrix();
	//色の指定
	materialData_->color = color;
	ImGui::Checkbox("useMonsterBallSphere", &useMonsterBall);
}

void Sphere::Draw() {

	directX12_->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView);	//VBVを設定
	//形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えておけばよい
	directX12_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	directX12_->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
	//wvp用のCBufferの場所を設定
	directX12_->GetCommandList()->SetGraphicsRootConstantBufferView(1, wvpResource_->GetGPUVirtualAddress());
	//SRV用のDescriptorTableの先頭を設定。2はrootParameter[2]である。
	directX12_->GetCommandList()->SetGraphicsRootDescriptorTable(2, useMonsterBall ? directX12_->GetTextureSrvHandleGPU2() : directX12_->GetTextureSrvHandleGPU());
	directX12_->GetCommandList()->SetGraphicsRootConstantBufferView(3, light_->GetDirectionalLightResource()->GetGPUVirtualAddress());

	//描画！　（DrawCall/ドローコール)。3頂点で1つのインスタンス。
	directX12_->GetCommandList()->DrawInstanced(vertexIndex_, 1, 0, 0);
}

