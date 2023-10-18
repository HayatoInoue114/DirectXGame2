#include "Triangle.h"

void Triangle::Initialize(DirectX12* directX12, TriangleData triangleData) {
	directX12_ = directX12;

	CreateVertexResource();
	CreateMaterialResource();
	CreateVertexBufferView();
	CreateTransformationMatrixResource();
	WriteDataToResource();

	//左下
	vertexData[0].position = triangleData.Left.position;
	vertexData[0].texcoord = triangleData.Left.texcoord;
	//上
	vertexData[1].position = triangleData.Top.position;
	vertexData[1].texcoord = triangleData.Top.texcoord;
	//右下
	vertexData[2].position = triangleData.Right.position;
	vertexData[2].texcoord = triangleData.Right.texcoord;

	//左下2
	vertexData[3].position = { -0.5f,-0.5f,0.5f,1.0f };
	vertexData[3].texcoord = { 0.0f,1.0f };
	//上2
	vertexData[4].position = { 0.0f,0.0f,0.0f,1.0f };
	vertexData[4].texcoord = { 0.5f,0.0f };
	//右下2
	vertexData[5].position = { 0.5f,-0.5f,-0.5f,1.0f };
	vertexData[5].texcoord = { 1.0f,1.0f };

	//Transform変数を作る
	transform_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
}

void Triangle::CreateVertexResource() {
	vertexResource = directX12_->CreateBufferResource(directX12_->GetDevice(), sizeof(VertexData) * 6);
}

void Triangle::CreateVertexBufferView() {
	vertexBufferView = {};
	//リソースの先頭のアドレスから使う
	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();
	//使用するリソースのサイズは頂点３つ分のサイズ
	vertexBufferView.SizeInBytes = sizeof(VertexData) * 6;
	//1頂点当たりのサイズ
	vertexBufferView.StrideInBytes = sizeof(VertexData);
}

void Triangle::CreateMaterialResource() {
	materialResource_ = directX12_->CreateBufferResource(directX12_->GetDevice(), sizeof(Vector4));
	materialData_ = nullptr;
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
}

void Triangle::CreateTransformationMatrixResource() {
	//WVP用のリソースを作る。Matrix4x4　1つ分のサイズを用意する
	wvpResource_ = directX12_->CreateBufferResource(directX12_->GetDevice(), sizeof(Matrix4x4));
	//データを書き込む
	wvpData_ = nullptr;
	//書き込むためのアドレスを取得
	wvpResource_->Map(0, nullptr, reinterpret_cast<void**>(&wvpData_));
	//単位行列を書き込んでおく
	*wvpData_ = MakeIdentity4x4();
}

void Triangle::WriteDataToResource() {
	//書き込むためのアドレスを取得
	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
}

void Triangle::CreateWVPMatrix() {
	cameraTransform_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,-5.0f,} };

	worldMatrix_ = MakeAffineMatrix(transform_.scale, transform_.rotate, transform_.translate);
	cameramatrix_ = MakeAffineMatrix(cameraTransform_.scale, cameraTransform_.rotate, cameraTransform_.translate);
	viewMatrix_ = Inverse(cameramatrix_);
	projectionMatix_ = MakePerspectiveFovMatrix(0.45f, float(kCliantWidth) / float(kCliantHeight), 0.1f, 100.0f);
	worldViewProjectionMatrix_ = Multiply(worldMatrix_, Multiply(viewMatrix_, projectionMatix_));
	*wvpData_ = worldViewProjectionMatrix_;
}

void Triangle::Release() {
	vertexResource->Release();
	materialResource_->Release();
}

void Triangle::Update(Transform transform, Vector4 color) {
	transform_ = transform;
	CreateWVPMatrix();
	//色の指定
	*materialData_ = color;
}

void Triangle::Draw() {

	directX12_->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView);	//VBVを設定
	//形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えておけばよい
	directX12_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	directX12_->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
	//wvp用のCBufferの場所を設定
	directX12_->GetCommandList()->SetGraphicsRootConstantBufferView(1, wvpResource_->GetGPUVirtualAddress());
	//SRV用のDescriptorTableの先頭を設定。2はrootParameter[2]である。
	directX12_->GetCommandList()->SetGraphicsRootDescriptorTable(2, directX12_->GetTextureSrvHandleGPU());
	//描画！　（DrawCall/ドローコール)。3頂点で1つのインスタンス。
	directX12_->GetCommandList()->DrawInstanced(6, 1, 0, 0);
}

