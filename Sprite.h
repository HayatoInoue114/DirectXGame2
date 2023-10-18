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

class Sprite
{
public:
	void Initialize();

	void Update();

	void Draw();

	void CreateVertexResource();

	void CreateVertexBufferView();

	void SetVertexData();

	void CreateTransformationMatrixResource();
private:
	DirectX12* directX12_;

	ID3D12Resource* vertexResourceSprite_{};

	D3D12_VERTEX_BUFFER_VIEW vertexBufferViewSprite_{};

	VertexData* vertexDataSprite_{};

	ID3D12Resource* transfoemationMatrixResourceSprite_{};

	Matrix4x4* transformationMatrixDataSprite_{};
};

