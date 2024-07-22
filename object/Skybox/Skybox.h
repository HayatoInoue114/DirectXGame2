#pragma once
//#include <dxcapi.h>
#include "../../base/DirectX12/DirectX12.h"
#include "../../math/Vector4.h"
#include "../../math/MT/MT.h"
#include "../../structure/VertexData/VertexData.h"
#include "../../structure/Material/Material.h"
#include "../../math/TransformationMatrix.h"
#include "../../math/WorldTransform/WorldTransform.h"
#include "../../manager/TextureManager/TextureManager.h"
#include "../Camera/Camera.h"

class Light;
class GraphicsRenderer;

class Skybox
{
public:
	void Initialize();

	void Draw();

	void ImGuiAdjustParameter();

	void SetPosition(const Vector3& pos) { transform_.translate = pos; }
	void SetScale(const Vector3& scale) { transform_.scale = scale; }

	bool isParent;

public:
	void SetCamera(Camera* camera) { camera_ = camera; }

private:
	void CreateVertexBufferView();

	void WriteDataToResource();

	void CreateMaterialResource();

	void CreateWVPMatrix();

	void SetMaterialData();

	void CreateModel();

	void CreateTransformationMatrixResource();

	void CreateResourceView();

private:
	//モデル読み込み
	ModelData modelData_{};

	Camera* camera_;
	Material* materialData_{};

	Light* light_{};

	DirectX12* directX12_{};

	//頂点リソース用のヒープの設定
	D3D12_HEAP_PROPERTIES uploadHeapProperties{};
	//頂点リソースの設定
	D3D12_RESOURCE_DESC vertexresourceDesc{};
	//実際に頂点リソースを作る
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource_{};
	//頂点バッファビューを作成する
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};

	//頂点リソースにデータを書き込む
	VertexData* vertexData_{};

	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource_{};

	D3D12_INDEX_BUFFER_VIEW indexBufferView_{};

	/*Vector4* materialData_;*/

	Microsoft::WRL::ComPtr<ID3D12Resource> wvpResource_{};
	TransformationMatrix* wvpData_{};

	WorldTransform transform_{};
	Matrix4x4 worldMatrix_{};

	WorldTransform cameraTransform_{};

	Matrix4x4 projectionMatrix_{};

	Matrix4x4 cameramatrix_{};

	Matrix4x4 viewMatrix_{};

	Matrix4x4 projectionMatix_{};

	Matrix4x4 worldViewProjectionMatrix_{};

	WorldTransform uvTransform_{};

	Matrix4x4 uvTransformMatrix_{};

	uint32_t* indexData_ = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> indexResource_{};

private:
	// 定数の頂点数とインデックス数
	const size_t kNumVertices_ = 24; // 各面4頂点、6面で計24頂点
	const size_t kNumIndices_ = 36;  // 各面6インデックス、6面で計36インデックス
};

