#pragma once
#include "DirectX12.h"
#include <dxcapi.h>
#include "Vector4.h"
#include "MT.h"

#pragma comment(lib,"dxcompiler.lib")



struct VertexData
{
	Vector4 position;
	Vector2 texcoord;
};

struct TriangleData
{
	VertexData Left;
	VertexData Top;
	VertexData Right;
};

class Triangle
{
public:
	void Initialize(DirectX12* directX12,TriangleData triangleData);

	void CreateVertexResource();

	void CreateVertexBufferView();

	void WriteDataToResource();

	void Release();

	void CreateMaterialResource();

	void CreateTransformationMatrixResource();
	
	void Update(Transform transform,Vector4 color);

	void Draw();

	void CreateWVPMatrix();
private:
	DirectX12* directX12_;

	//頂点リソース用のヒープの設定
	D3D12_HEAP_PROPERTIES uploadHeapProperties;
	//頂点リソースの設定
	D3D12_RESOURCE_DESC vertexresourceDesc;
	//実際に頂点リソースを作る
	ID3D12Resource* vertexResource;
	//頂点バッファビューを作成する
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView;

	//頂点リソースにデータを書き込む
	VertexData* vertexData;

	ID3D12Resource* materialResource_;

	Vector4* materialData_;

	ID3D12Resource* wvpResource_;
	Matrix4x4* wvpData_;

	Transform transform_;
	Matrix4x4 worldMatrix_;

	Transform cameraTransform_;
	
	Matrix4x4 projectionMatrix_;

	Matrix4x4 cameramatrix_;

	Matrix4x4 viewMatrix_;

	Matrix4x4 projectionMatix_;

	Matrix4x4 worldViewProjectionMatrix_;
};


