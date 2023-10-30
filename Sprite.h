#pragma once
#include "DirectX12.h"
#include <dxcapi.h>
#include "Vector4.h"
#include "MT.h"
#include "Vertexdata.h"
#include "Material.h"

#pragma comment(lib,"dxcompiler.lib")

class Sprite
{
public:
	void Initialize(DirectX12* directX12);

	void Update(Transform& transform, Vector4& color);

	void Draw();

	void CreateVertexResource();

	void CreateVertexBufferView();

	void SetVertexData();

	void CreateTransformationMatrixResource();

	void CalculateAndSetWVPMatrix();

	void CreateMaterialResource();

	//void Release();
private:
	Material* materialData_{};

	DirectX12* directX12_;

	ID3D12Resource* vertexResourceSprite_{};

	D3D12_VERTEX_BUFFER_VIEW vertexBufferViewSprite_{};

	VertexData* vertexDataSprite_{};

	ID3D12Resource* transformationMatrixResourceSprite_{};

	Matrix4x4* transformationMatrixDataSprite_{};

	Transform transform_;

	Matrix4x4 worldMatrixSprite_{};

	Matrix4x4 viewMatrixSprite_{};

	Matrix4x4 projectionMatrixSprite_{};

	Matrix4x4 worldViewProjectionMatrixSprite_{};

	Transform cameraTransform_{};

	Matrix4x4 cameramatrix_{};

	ID3D12Resource* materialResource_{};
};

