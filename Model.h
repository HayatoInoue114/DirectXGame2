#pragma once
#include "ModelData.h"
#include "MT.h"
#include <string>
#include <fstream>
#include <sstream>
#include "DirectX12.h"
#include "Light.h"
#include "Material.h"
#include "TransformationMatrix.h"
#include "TextureManager.h"

class Model
{
public:
	void Initialize(DirectX12* directX12, Light* light);

	void Update(Transform& transform, Vector4& color);

	void Draw(uint32_t textureNum);

	void CreatevertexResource();

	void CreateVertexBufferView();

	void WriteDataToResource();

	void CreateMaterialResource();

	void CreateTransformationMatrixResource();

	void CreateWVPMatrix();

	void SetMaterialData();

	void ImGuiAdjustParameter();

	void CreateModel();

	ModelData LoadObjFile(const std::string& directorypath, const std::string& filename);
private:
	//モデル読み込み
	ModelData modelData_{};

	Material* materialData_{};

	Light* light_{};

	DirectX12* directX12_{};

	//頂点リソース用のヒープの設定
	D3D12_HEAP_PROPERTIES uploadHeapProperties{};
	//頂点リソースの設定
	D3D12_RESOURCE_DESC vertexResource_Desc{};
	//実際に頂点リソースを作る
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource_{};
	//頂点バッファビューを作成する
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};

	//頂点リソースにデータを書き込む
	VertexData* vertexData_{};

	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource_{};

	ID3D12Resource* wvpResource_{};
	TransformationMatrix* wvpData_{};

	Transform transform_{};
	Matrix4x4 worldMatrix_{};

	Transform cameraTransform_{};

	Matrix4x4 projectionMatrix_{};

	Matrix4x4 cameramatrix_{};

	Matrix4x4 viewMatrix_{};

	Matrix4x4 projectionMatix_{};

	Matrix4x4 worldViewProjectionMatrix_{};

	Transform uvTransform_{};

	Matrix4x4 uvTransformMatrix_{};
};

