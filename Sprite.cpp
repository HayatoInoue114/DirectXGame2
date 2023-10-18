#include "Sprite.h"

void Sprite::Initialize(DirectX12* directX12) {
	directX12_ = directX12;

	CreateVertexResource();
	CreateVertexBufferView();
	SetVertexData();
	CreateTransformationMatrixResource();
	//Transform変数を作る
	transform_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	CalculateAndSetWVPMatrix();
}

void Sprite::Update(Transform& transform, Vector4& color) {
	transform_ = transform;
	worldMatrixSprite_ = MakeAffineMatrix(transform_.scale, transform_.rotate, transform_.translate);
	*transformationMatrixDataSprite_ = worldMatrixSprite_;
	//色の指定
	//*materialData_ = color;
}

void Sprite::CreateVertexResource() {
	//Sprite用の頂点リソースを作る
	vertexResourceSprite_ = directX12_->CreateBufferResource(directX12_->GetDevice(), sizeof(VertexData) * 6);
}

void Sprite::CreateVertexBufferView() {
	//頂点バッファビューを作成する
	vertexBufferViewSprite_ = {};

	//リソースの先頭のアドレスから使う
	vertexBufferViewSprite_.BufferLocation = vertexResourceSprite_->GetGPUVirtualAddress();

	//使用するリソースのサイズは頂点6つ分のサイズ
	vertexBufferViewSprite_.SizeInBytes = sizeof(VertexData) * 6;
	//1頂点当たりのサイズ
	vertexBufferViewSprite_.StrideInBytes = sizeof(VertexData);
}

void Sprite::SetVertexData() {
	vertexDataSprite_ = nullptr;
	vertexResourceSprite_->Map(0, nullptr, reinterpret_cast<void**>(&vertexDataSprite_));

	//１枚目の三角形
	vertexDataSprite_[0].position = { 0.0f, 360.0f, 0.0f, 1.0f };// 左下
	vertexDataSprite_[0].texcoord = { 0.0f,1.0f };
	vertexDataSprite_[1].position = { 0.0f, 0.0f, 0.0f, 1.0f };// 左上
	vertexDataSprite_[1].texcoord = { 0.0f,0.0f };
	vertexDataSprite_[2].position = { 640.0f, 360.0f, 0.0f, 1.0f };// 右下
	vertexDataSprite_[2].texcoord = { 1.0f,1.0f };
	//2枚目の三角形
	vertexDataSprite_[0].position = { 0.0f, 0.0f, 0.0f, 1.0f };// 左上
	vertexDataSprite_[0].texcoord = { 0.0f,0.0f };
	vertexDataSprite_[1].position = { 640.0f, 0.0f, 0.0f, 1.0f };// 右上
	vertexDataSprite_[1].texcoord = { 1.0f,0.0f };
	vertexDataSprite_[2].position = { 640.0f, 360.0f, 0.0f, 1.0f };// 右下
	vertexDataSprite_[2].texcoord = { 1.0f,1.0f };
}

void Sprite::CreateTransformationMatrixResource() {
	//WVP用のリソースを作る。Matrix4x4　1つ分のサイズを用意する
	transformationMatrixResourceSprite_ = directX12_->CreateBufferResource(directX12_->GetDevice(), sizeof(Matrix4x4));
	//データを書き込む
	transformationMatrixDataSprite_ = nullptr;
	//書き込むためのアドレスを取得
	transformationMatrixResourceSprite_->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrixDataSprite_));
	//単位行列を書き込んでおく
	*transformationMatrixDataSprite_ = MakeIdentity4x4();
}

void Sprite::CalculateAndSetWVPMatrix() {
	worldMatrixSprite_ = MakeAffineMatrix(transform_.scale, transform_.rotate, transform_.translate);
	viewMatrixSprite_ = MakeIdentity4x4();
	projectionMatrixSprite_ = MakeOrthographicMatrix(0.0f, 0.0f, float(kCliantWidth), float(kCliantHeight), 0.0f, 100.0f);
	worldViewProjectionMatrixSprite_ = Multiply(worldMatrixSprite_, Multiply(viewMatrixSprite_, projectionMatrixSprite_));
	*transformationMatrixDataSprite_ = worldViewProjectionMatrixSprite_;
}

void Sprite::Draw() {
	//Spriteの描画。変更が必要なものだけを変更する
	directX12_->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferViewSprite_); // VBVを設定
	//TransformationMatrixCBufferの場所を設定
	directX12_->GetCommandList()->SetComputeRootConstantBufferView(1, transformationMatrixResourceSprite_->GetGPUVirtualAddress());
	//描画！(DrawCall/ドローコール)
	directX12_->GetCommandList()->DrawInstanced(6, 1, 0, 0);
}
