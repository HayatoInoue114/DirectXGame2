#include "Sprite.h"

void Sprite::Initialize(DirectX12* directX12) {
	directX12_ = directX12;

	CreateVertexResource();
	CreateMaterialResource();
	CreateVertexBufferView();
	SetVertexData();
	CreateTransformationMatrixResource();
	CreateIndex();
	//Transform変数を作る
	transform_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };

	// 色の設定
	materialData_->color = { 1.0f,1.0f,1.0f,1.0f };
	// Lightingを有効にする
	materialData_->enableLighting = false;
}

void Sprite::CreateVertexResource() {
	//Sprite用の頂点リソースを作る
	vertexResource_ = directX12_->CreateBufferResource(directX12_->GetDevice(), sizeof(VertexData) * 6);
}

void Sprite::CreateVertexBufferView() {
	//頂点バッファビューを作成する
	vertexBufferView_ = {};

	//リソースの先頭のアドレスから使う
	vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();

	//使用するリソースのサイズは頂点6つ分のサイズ
	vertexBufferView_.SizeInBytes = sizeof(VertexData) * 6;
	//1頂点当たりのサイズ
	vertexBufferView_.StrideInBytes = sizeof(VertexData);
}

void Sprite::SetVertexData() {
	vertexDataSprite_ = nullptr;
	vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexDataSprite_));

	//１枚目の三角形
	vertexDataSprite_[0].position = { 0.0f, 360.0f, 0.0f, 1.0f };// 左下
	vertexDataSprite_[0].texcoord = { 0.0f,1.0f };
	vertexDataSprite_[0].normal = { 0.0f,0.0f,-1.0f };
	vertexDataSprite_[1].position = { 0.0f, 0.0f, 0.0f, 1.0f };// 左上
	vertexDataSprite_[1].texcoord = { 0.0f,0.0f };
	vertexDataSprite_[1].normal = { 0.0f,0.0f,-1.0f };
	vertexDataSprite_[2].position = { 640.0f, 360.0f, 0.0f, 1.0f };// 右下
	vertexDataSprite_[2].texcoord = { 1.0f,1.0f };
	vertexDataSprite_[2].normal = { 0.0f,0.0f,-1.0f };
	//2枚目の三角形
	vertexDataSprite_[3].position = { 0.0f, 0.0f, 0.0f, 1.0f };// 左上
	vertexDataSprite_[3].texcoord = { 0.0f,0.0f };
	vertexDataSprite_[3].normal = { 0.0f,0.0f,-1.0f };
	//vertexDataSprite_[4].position = { 640.0f, 0.0f, 0.0f, 1.0f };// 右上
	//vertexDataSprite_[4].texcoord = { 1.0f,0.0f };
	//vertexDataSprite_[4].normal = { 0.0f,0.0f,-1.0f };
	//vertexDataSprite_[5].position = { 640.0f, 360.0f, 0.0f, 1.0f };// 右下
	//vertexDataSprite_[5].texcoord = { 1.0f,1.0f };
	//vertexDataSprite_[5].normal = { 0.0f,0.0f,-1.0f };
}

void Sprite::CreateTransformationMatrixResource() {
	//WVP用のリソースを作る。Matrix4x4　1つ分のサイズを用意する
	transformationMatrixResource_ = directX12_->CreateBufferResource(directX12_->GetDevice(), sizeof(TransformationMatrix));
	//データを書き込む
	transformationMatrixData_ = nullptr;
	//書き込むためのアドレスを取得
	transformationMatrixResource_->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrixData_));
	//単位行列を書き込んでおく
	transformationMatrixData_->WVP = MakeIdentity4x4();
	transformationMatrixData_->World = MakeIdentity4x4();
}

void Sprite::CalculateAndSetWVPMatrix() {
	cameraTransform_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,-10.0f,} };

	worldMatrix_ = MakeAffineMatrix(transform_.scale, transform_.rotate, transform_.translate);
	cameramatrix_ = MakeAffineMatrix(cameraTransform_.scale, cameraTransform_.rotate, cameraTransform_.translate);
	viewMatrix_ = MakeIdentity4x4();
	projectionMatrix_ = MakeOrthographicMatrix(0.0f, 0.0f, float(kCliantWidth), float(kCliantHeight), 0.0f, 100.0f);
	worldViewprojectionMatrix_ = Multiply(worldMatrix_, Multiply(viewMatrix_, projectionMatrix_));
	transformationMatrixData_->WVP = worldViewprojectionMatrix_;
	transformationMatrixData_->World = worldMatrix_;

	//projectionMatix_ = MakePerspectiveFovMatrix(0.45f, float(kCliantWidth) / float(kCliantHeight), 0.1f, 100.0f);
}

void Sprite::CreateMaterialResource() {
	//Sprite用のマテリアルリソースを作る
	materialResource_ = directX12_->CreateBufferResource(directX12_->GetDevice(), sizeof(Material));
	materialData_ = nullptr;
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));

}

void Sprite::CreateIndex() {
	indexResource_ = directX12_->CreateBufferResource(directX12_->GetDevice(), sizeof(uint32_t) * 6);

	indexBufferView_ = {};
	//リソースの先頭アドレスから使う
	indexBufferView_.BufferLocation = indexResource_->GetGPUVirtualAddress();
	//使用するリソースのサイズはインデックス6つ分のサイズ
	indexBufferView_.SizeInBytes = sizeof(uint32_t) * 6;
	//インデックスはuint32_tとする
	indexBufferView_.Format = DXGI_FORMAT_R32_UINT;

	//インデックスリソースにデータを書き込む
	indexData_ = nullptr;
	indexResource_->Map(0, nullptr, reinterpret_cast<void**>(&indexData_));
	indexData_[0] = 0; indexData_[1] = 1; indexData_[2] = 2;
	indexData_[3] = 1; indexData_[4] = 3; indexData_[5] = 2;
}

void Sprite::Update(Transform& transform, Vector4& color) {
	transform_ = transform;
	CalculateAndSetWVPMatrix();
	//色の指定
	//materialData_->color = color;
}

void Sprite::Draw() {
	//Spriteの描画。変更が必要なものだけを変更する
	directX12_->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView_); // VBVを設定
	directX12_->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
	//wvp用のCBufferの場所を設定
	//directX12_->GetCommandList()->SetGraphicsRootConstantBufferView(1, wvpResource_->GetGPUVirtualAddress());
	//TransformationMatrixCBufferの場所を設定
	directX12_->GetCommandList()->SetGraphicsRootConstantBufferView(1, transformationMatrixResource_->GetGPUVirtualAddress());
	//IBVを設定
	directX12_->GetCommandList()->IASetIndexBuffer(&indexBufferView_);
	//描画！(DrawCall/ドローコール)
	directX12_->GetCommandList()->DrawInstanced(6, 1, 0, 0);
}

//void Sprite::Release() {
//	vertexResource->Release();
//	materialResource_->Release();
//}