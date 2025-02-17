#include "Sprite.h"
#include "../../base/GraphicsRenderer/GraphicsRenderer.h"
#include "../../manager/TextureManager/TextureManager.h"
#include "../../manager/SrvManager/SrvManager.h"

/// <summary>
/// スプライト生成
/// </summary>
/// <param name="position"></param>
/// <param name="size"></param>
/// <param name="color"></param>
/// <returns></returns>
Sprite* Sprite::Create(Vector3 position, Vector2 size, Vector4 color, uint32_t textureNum)
{
	Sprite* sprite = new Sprite;
	sprite->Initialize();
	sprite->SetSize(size);
	sprite->SetPosition({ position.x,position.y });
	sprite->SetColor(color);
	sprite->SetTextureNum(textureNum);
	//textureNum_ = textureNum;

	return sprite;
}

std::unique_ptr<Sprite> Sprite::CreateUnique(Vector3 position, Vector2 size, Vector4 color, const std::string& filename) {
	std::unique_ptr<Sprite> sprite;
	sprite = std::make_unique<Sprite>();
	sprite->Initialize();
	sprite->SetSize(size);
	sprite->SetPosition({ position.x,position.y });
	sprite->SetColor(color);
	sprite->SetFileName(filename);
	TextureManager::GetInstance()->LoadTexture("", filename);
	//textureNum_ = textureNum;

	return sprite;
}

void Sprite::Initialize() {
	directX12_ = DirectX12::GetInstance();


	//cameraTransform_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,-5.0f,} };
	cameraTransform_.Initialize();
	cameraTransform_.translate = { 0.0f,0.0f,-5.0f };
	//Transform変数を作る
	worldTransform_.Initialize();

	size_ = { 640,360 };

	textureNum_ = 0;

	CreateVertexResource();
	CreateMaterialResource();
	CreateVertexBufferView();
	vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));

	SetVertexData();
	CreateTransformationMatrixResource();
	CreateIndex();
	SetMaterialData();
}

void Sprite::CreateVertexResource() {
	//Sprite用の頂点リソースを作る
	vertexResource_ = directX12_->CreateBufferResource(directX12_->GetDevice().Get(), sizeof(VertexData) * 6);
}

void Sprite::CreateVertexBufferView() {
	//頂点バッファビューを作成する
	vertexBufferView_ = {};

	//リソースの先頭のアドレスから使う
	vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();

	//使用するリソースのサイズは頂点6つ分のサイズ
	vertexBufferView_.SizeInBytes = sizeof(VertexData) * 4;
	//1頂点当たりのサイズ
	vertexBufferView_.StrideInBytes = sizeof(VertexData);
}

void Sprite::SetVertexData() {

	float left = 0.0f - anchorPoint_.x;
	float right = 1.0f - anchorPoint_.x;
	float top = 0.0f - anchorPoint_.y;
	float bottom = 1.0f - anchorPoint_.y;


	// 左下
	vertexData_[LB].position = { left, bottom, 0.0f, 1.0f };
	vertexData_[LB].texcoord = { 0.0f,1.0f };
	vertexData_[LB].normal = { 0.0f,0.0f,-1.0f };
	// 左上
	vertexData_[LT].position = { left, top, 0.0f, 1.0f };
	vertexData_[LT].texcoord = { 0.0f,0.0f };
	vertexData_[LT].normal = { 0.0f,0.0f,-1.0f };
	// 右下
	vertexData_[RB].position = { right, bottom, 0.0f, 1.0f };
	vertexData_[RB].texcoord = { 1.0f,1.0f };
	vertexData_[RB].normal = { 0.0f,0.0f,-1.0f };
	// 右上
	vertexData_[RT].position = { right, top, 0.0f, 1.0f };
	vertexData_[RT].texcoord = { 1.0f, 0.0f };
	vertexData_[RT].normal = { 0.0f,0.0f,-1.0f };
}

void Sprite::CreateTransformationMatrixResource() {
	//WVP用のリソースを作る。Matrix4x4　1つ分のサイズを用意する
	transformationMatrixResource_ = directX12_->CreateBufferResource(directX12_->GetDevice().Get(), sizeof(TransformationMatrix));
	//データを書き込む
	transformationMatrixData_ = nullptr;
	//書き込むためのアドレスを取得
	transformationMatrixResource_->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrixData_));
	//単位行列を書き込んでおく
	transformationMatrixData_->WVP = MakeIdentity4x4();
	transformationMatrixData_->World = MakeIdentity4x4();
}

void Sprite::CalculateAndSetWVPMatrix() {

	transformationMatrixData_->World = MakeAffineMatrix(worldTransform_.scale, worldTransform_.rotate, worldTransform_.translate);
	cameraMatrix_ = MakeAffineMatrix(cameraTransform_.scale, cameraTransform_.rotate, cameraTransform_.translate);
	viewMatrix_ = MakeIdentity4x4();
	projectionMatrix_ = MakeOrthographicMatrix(0.0f, 0.0f, float(kCliantWidth), float(kCliantHeight), 0.0f, 100.0f);
	worldViewProjectionMatrix_ = Multiply(transformationMatrixData_->World, Multiply(viewMatrix_, projectionMatrix_));
	transformationMatrixData_->WVP = worldViewProjectionMatrix_;
	//transformationMatrixData_->World = worldMatrix_;

	//projectionMatrix_ = MakePerspectiveFovMatrix(0.45f, float(kClientWidth) / float(kClientHeight), 0.1f, 100.0f);
}

void Sprite::CreateMaterialResource() {
	//Sprite用のマテリアルリソースを作る
	materialResource_ = directX12_->CreateBufferResource(directX12_->GetDevice().Get(), sizeof(Material));
	materialData_ = nullptr;
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
}

void Sprite::CreateIndex() {
	indexResource_ = directX12_->CreateBufferResource(directX12_->GetDevice().Get(), sizeof(uint32_t) * 6);

	indexBufferView_ = {};
	//リソースの先頭アドレスから使う
	indexBufferView_.BufferLocation = indexResource_->GetGPUVirtualAddress();
	//使用するリソースのサイズはインデックス6つ分のサイズ
	indexBufferView_.SizeInBytes = sizeof(uint32_t) * 6;
	//インデックスは:uint32_tとする
	indexBufferView_.Format = DXGI_FORMAT_R32_UINT;

	//インデックスリソースにデータを書き込む
	indexData_ = nullptr;
	indexResource_->Map(0, nullptr, reinterpret_cast<void**>(&indexData_));
	indexData_[0] = 0; indexData_[1] = 1; indexData_[2] = 2;
	indexData_[3] = 1; indexData_[4] = 3; indexData_[5] = 2;
}

void Sprite::SetMaterialData() {
	// 色の設定
	materialData_->color = { 1.0f,1.0f,1.0f,1.0f };
	// Lightingを有効にする
	materialData_->enableLighting = false;
	//UVTransformを単位行列で初期化
	materialData_->uvTransform = MakeIdentity4x4();
	//uvTransform用の変数
	uvTransform_.Initialize();
}

void Sprite::Update() {
	worldTransform_.scale = { size_.x,size_.y,1.0f };

	
	
	//色の指定
	//ImGuiAdjustParameter();

}

void Sprite::Draw() {
	GraphicsRenderer::GetInstance()->SetRootSignatureAndPSO(0);
	if (isInvisible_) {
		return;
	}
	SetVertexData();

	CalculateAndSetWVPMatrix();
	//パラメータからUVTransform用の行列を生成する
	uvTransformMatrix_ = MakeScaleMatrix(uvTransform_.scale);
	uvTransformMatrix_ = Multiply(uvTransformMatrix_, MakeRotateZMatrix(uvTransform_.rotate.z));
	uvTransformMatrix_ = Multiply(uvTransformMatrix_, MakeTranslateMatrix(uvTransform_.translate));
	materialData_->uvTransform = uvTransformMatrix_;

	//Spriteの描画。変更が必要なものだけを変更する
	directX12_->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView_); // VBVを設定
	//IBVを設定
	directX12_->GetCommandList()->IASetIndexBuffer(&indexBufferView_);

	directX12_->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
	//wvp用のCBufferの場所を設定
	//TransformationMatrixCBufferの場所を設定
	directX12_->GetCommandList()->SetGraphicsRootConstantBufferView(1, transformationMatrixResource_->GetGPUVirtualAddress());
	uint32_t index = TextureManager::GetInstance()->GetSrvIndex("resources/" + filename_);
	SrvManager::GetInstance()->SetGraphicsRootDescriptorTable(2, index);
	//描画！(DrawCall/ドローコール)
	directX12_->GetCommandList()->DrawIndexedInstanced(6, 1, 0, 0, 0);
}

void Sprite::ImGuiAdjustParameter() {
	ImGui::Begin("Sprite");
	//ImGui::Text("Sprite");
	ImGui::CheckboxFlags("isLighting", &materialData_->enableLighting, 1);
	ImGui::SliderFloat3("Translate", &worldTransform_.translate.x, -5, 5);
	ImGui::SliderFloat3("Scale", &worldTransform_.scale.x, -5, 5);
	ImGui::SliderFloat3("Rotate", &worldTransform_.rotate.x, -5, 5);
	ImGui::Text("UVTransform");
	ImGui::DragFloat2("UVTranslate", &uvTransform_.translate.x, 0.01f, -10.0f, 10.0f);
	ImGui::DragFloat2("UVScale", &uvTransform_.scale.x, 0.01f, -10.0f, 10.0f);
	ImGui::SliderAngle("UVRotate.z", &uvTransform_.rotate.z);
	ImGui::ColorEdit4("ModelColor", &materialData_->color.x, 1);
	ImGui::End();
}