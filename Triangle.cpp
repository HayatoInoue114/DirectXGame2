#include "Triangle.h"

void Triangle::Initialize(DirectX12* directX12, const TriangleData& triangleData) {
	directX12_ = directX12;
	CreateVertexResource();
	CreateMaterialResource();
	CreateVertexBufferView();
	WriteDataToResource();

	//左下
	vertexData[0] = left;
	//上
	vertexData[1] = top;
	//右下
	vertexData[2] = right;
}

void Triangle::CreateVertexResource() {
	vertexResource = directX12_->CreateBufferResource(directX12_->GetDevice(), sizeof(Vector4) * 3);
}

void Triangle::CreateVertexBufferView() {
	vertexBufferView = {};
	//リソースの先頭のアドレスから使う
	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();
	//使用するリソースのサイズは頂点３つ分のサイズ
	vertexBufferView.SizeInBytes = sizeof(Vector4) * 3;
	//1頂点当たりのサイズ
	vertexBufferView.StrideInBytes = sizeof(Vector4);
}

void Triangle::CreateMaterialResource() {
	materialResource_ = directX12_->CreateBufferResource(directX12_->GetDevice(), sizeof(Vector4));
	materialData_ = nullptr;
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
	*materialData_ = Vector4(1.0f, 0.0f, 0.0f, 1.0f);
}

void Triangle::WriteDataToResource() {
	//書き込むためのアドレスを取得
	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
}

void Triangle::Release() {
	vertexResource->Release();
	materialResource_->Release();
}

void Triangle::Draw() {
	

	directX12_->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView);	//VBVを設定
	//形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えておけばよい
	directX12_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	directX12_->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
	//描画！　（DrawCall/ドローコール)。3頂点で1つのインスタンス。インスタンスについては今後
	directX12_->GetCommandList()->DrawInstanced(3, 1, 0, 0);
}