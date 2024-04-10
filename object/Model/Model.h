#pragma once
#include <string>
#include <fstream>
#include <sstream>

#include "../../base/DirectX12/DirectX12.h"
#include "../../math/Vector4.h"
#include "../../math/MT/MT.h"
#include "../../structure/VertexData/VertexData.h"
#include "../../structure/Material/Material.h"
#include "../../components/Light/Light.h"
#include "../../manager/TextureManager/TextureManager.h"
#include "../../structure/ModelData/ModelData.h"
#include "../../math/WorldTransform/WorldTransform.h"
#include "../../manager/ModelManager/ModelManager.h"
#include "../../math/Transform/Transform.h"

class Model
{
public:

	void Initialize();

	void Update();

	void Draw();

	void ImGuiAdjustParameter();

	ModelData LoadObjFile(const std::string& directorypath, const std::string& filename);

	static Model* CreateModelFromObj(int modelName);

	static std::unique_ptr<Model> CreateModelFromObjPtr(int modelName);

	

public:  //ゲッター,セッター
	void SetTransform(const TransformS& transform) { worldTransform_.scale = transform.scale,worldTransform_.rotate = transform.rotate,worldTransform_.translate = transform.translate; }
	void SetWorldTransform(const WorldTransform& worldTransform) { worldTransform_ = worldTransform; }
	void SetWVP(const TransformationMatrix& wvp) { *wvpData_ = wvp; }

	Microsoft::WRL::ComPtr<ID3D12Resource> GetInstancingResource() { return instancingResource_; }
	void SetTextureNum(uint32_t num) { modelName_ = num; }
	void SetColor(const Vector4& color) { materialData_->color = color; }

private:
	//void CreateVertexResource();

	void CreateVertexBufferView();

	void WriteDataToResource();

	void CreateMaterialResource();

	void CreateTransformationMatrixResource();

	void CreateWVPMatrix();

	void SetMaterialData();

	void CreateModel();

private:
	//モデル読み込み
	ModelData modelData_{};

	Material* materialData_{};

	//頂点リソース用のヒープの設定
	D3D12_HEAP_PROPERTIES uploadHeapProperties_{};
	//頂点リソースの設定
	D3D12_RESOURCE_DESC vertexResourceDesc_{};
	//実際に頂点リソースを作る
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource_{};
	//頂点バッファビューを作成する
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};

	//頂点リソースにデータを書き込む
	VertexData* vertexData_{};

	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource_{};

	ID3D12Resource* wvpResource_{};
	TransformationMatrix* wvpData_{};

	//Transform transform_{};
	WorldTransform worldTransform_{};

	Matrix4x4 worldMatrix_{};

	WorldTransform cameraTransform_{};

	Matrix4x4 projectionMatrix_{};

	Matrix4x4 cameraMatrix_{};

	Matrix4x4 viewMatrix_{};

	Matrix4x4 worldViewProjectionMatrix_{};

	WorldTransform uvTransform_{};

	Matrix4x4 uvTransformMatrix_{};

	Microsoft::WRL::ComPtr<ID3D12Resource> instancingResource_;

	TransformationMatrix* instancingData_{};

	uint32_t descriptorSizeSRV_{};

	D3D12_CPU_DESCRIPTOR_HANDLE instancingSrvHandleCPU_{};

	D3D12_GPU_DESCRIPTOR_HANDLE instancingSrvHandleGPU_{};

	uint32_t modelName_ = {};
};

