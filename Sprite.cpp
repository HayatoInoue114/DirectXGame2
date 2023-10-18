#include "Sprite.h"

void Sprite::Initialize() {

}

void Sprite::Update() {

}

void Sprite::Draw() {

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
	vertexDataSprite_[0].position = { 0.0f, 0.0f, 0.0f, 1.0f };// 左下
	vertexDataSprite_[0].texcoord = { 0.0f,0.0f };
	vertexDataSprite_[1].position = { 640.0f, 0.0f, 0.0f, 1.0f };// 左上
	vertexDataSprite_[1].texcoord = { 1.0f,0.0f };
	vertexDataSprite_[2].position = { 640.0f, 360.0f, 0.0f, 1.0f };// 右下
	vertexDataSprite_[2].texcoord = { 1.0f,1.0f };
}

void Sprite::CreateTransformationMatrixResource() {
	//WVP用のリソースを作る。Matrix4x4　1つ分のサイズを用意する
	transfoemationMatrixResourceSprite_ = directX12_->CreateBufferResource(directX12_->GetDevice(), sizeof(Matrix4x4));
	//データを書き込む
	transformationMatrixDataSprite_ = nullptr;
	//書き込むためのアドレスを取得
	transfoemationMatrixResourceSprite_->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrixDataSprite_));
	//単位行列を書き込んでおく
	*transformationMatrixDataSprite_ = MakeIdentity4x4();
}