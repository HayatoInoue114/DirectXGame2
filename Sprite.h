#pragma once
#include "DirectX12.h"
#include <dxcapi.h>
#include "Vector4.h"
#include "MT.h"
#include "Vertexdata.h"
#include "Material.h"
#include "TransformationMatrix.h"

#pragma comment(lib,"dxcompiler.lib")

class Sprite
{
public:
	void Initialize();

	void Update(Transform& transform, Vector4& color);

	void Draw();

	void CreateVertexResource();

	void CreateVertexBufferView();

	void SetVertexData();

	void CreateTransformationMatrixResource();

	void CalculateAndSetWVPMatrix();

	void CreateMaterialResource();

	void CreateIndex();

	void SetMaterialData();

	void ImGuiAdjustParameter();
private:
	Material* materialData_{};

	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource_{};

	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};

	VertexData* vertexData_{};

	Microsoft::WRL::ComPtr<ID3D12Resource> transformationMatrixResource_{};

	TransformationMatrix* transformationMatrixData_{};

	Transform transform_{};

	Matrix4x4 worldMatrix_{};

	Matrix4x4 viewMatrix_{};

	Matrix4x4 projectionMatrix_{};

	Matrix4x4 worldViewprojectionMatrix_{};

	Transform cameraTransform_{};

	Matrix4x4 cameramatrix_{};

	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource_;

	Microsoft::WRL::ComPtr<ID3D12Resource> indexResource_{};
	
	D3D12_INDEX_BUFFER_VIEW indexBufferView_{};

	uint32_t* indexData_ = nullptr;

	Transform uvTransform_{};

	Matrix4x4 uvTransformMatrix_{};
};

