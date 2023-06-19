#include "Triangle.h"

void Triangle::Initialize(DirectX12* directX12) {
	directX12_ = directX12;
	CreateVertexResource();
	CreateVertexBufferView();
	WriteDataToResource();
}

void Triangle::CreateVertexResource() {
	directX12_->CreateMaterialResource();
}

void Triangle::CreateVertexBufferView() {
	vertexBufferView = {};
	//リソースの先頭のアドレスから使う
	vertexBufferView.BufferLocation = directX12_->GetVertexResource()->GetGPUVirtualAddress();
	//使用するリソースのサイズは頂点３つ分のサイズ
	vertexBufferView.SizeInBytes = sizeof(Vector4) * 3;
	//1頂点当たりのサイズ
	vertexBufferView.StrideInBytes = sizeof(Vector4);
}

void Triangle::WriteDataToResource() {
	//書き込むためのアドレスを取得
	directX12_->GetVertexResource()->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
}

void Triangle::Release() {
	
}

void Triangle::Draw(Vector4 left ,Vector4 top,Vector4 right) {
	//左下
	vertexData[0] = left;
	//上
	vertexData[1] = top;
	//右下
	vertexData[2] = right;

	directX12_->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView);	//VBVを設定
	//形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えておけばよい
	directX12_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//マテリアルCBufferの場所を設定
	directX12_->SetCBV();
	//描画！　（DrawCall/ドローコール)。3頂点で1つのインスタンス。インスタンスについては今後
	directX12_->GetCommandList()->DrawInstanced(3, 1, 0, 0);
}