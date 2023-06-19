#pragma once
#include "DirectX12.h"
#include <dxcapi.h>
#include "Vector4.h"

#pragma comment(lib,"dxcompiler.lib")


class Triangle
{
public:
	void Initialize(DirectX12* directX12);

	void CreateVertexResource();

	void CreateVertexBufferView();

	void WriteDataToResource();

	void Release();

	void Draw(Vector4 left, Vector4 top, Vector4 right);
private:
	DirectX12* directX12_;


	//頂点バッファビューを作成する
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView;

	//頂点リソースにデータを書き込む
	Vector4* vertexData;


};


