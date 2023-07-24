#pragma once
#include "DirectX12.h"
#include <dxcapi.h>
#include "Vector4.h"
#include "MT.h"

#pragma comment(lib,"dxcompiler.lib")

class Triangle
{
public:
	void Initialize(DirectX12* directX12);

	void CreateVertexResource();

	void CreateVertexBufferView();

	void WriteDataToResource();

	void Release();

	void CreateMaterialResource();

	void CreateTransformationMatrixResource();
	
	void Update();

	void Draw(Vector4 left, Vector4 top, Vector4 right);
private:
	DirectX12* directX12_;

	//頂点リソース用のヒープの設定
	D3D12_HEAP_PROPERTIES uploadHeapProperties;
	//頂点リソースの設定
	D3D12_RESOURCE_DESC vertexResourceDesc;
	//実際に頂点リソースを作る
	ID3D12Resource* vertexResource;
	//頂点バッファビューを作成する
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView;

	//頂点リソースにデータを書き込む
	Vector4* vertexData;

	ID3D12Resource* materialResource_;

	Vector4* materialData_;

	ID3D12Resource* wvpResource_;
	Matrix4x4* wvpData_;

	Transform transform_;
	Matrix4x4 worldMatrix_;
	
};

