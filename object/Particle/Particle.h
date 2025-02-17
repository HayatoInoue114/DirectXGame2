#pragma once
#include <string>
#include <fstream>
#include <sstream>

#include "../../base/DirectX12/DirectX12.h"
#include "../../math/Vector4.h"
#include "../../math/MyMath.h"
#include "../../structure/VertexData/VertexData.h"
#include "../../structure/Material/Material.h"
#include "../../math/TransformationMatrix.h"
#include "../../components/Light/Light.h"
#include "../../manager/TextureManager/TextureManager.h"
#include "../../structure/ModelData/ModelData.h"
#include "../../math/WorldTransform/WorldTransform.h"
#include "../../structure/structure.h"
#include "../Camera/Camera.h"
#include "../../manager/SrvManager/SrvManager.h"

class GraphicsRenderer;

#define MAXINSTANCE 10 // インスタンス数

class Particle
{
public:
	void Initialize();

	void Update();

	void Draw(Camera* camera, uint32_t textureNum);

	void LoadModel(const std::string& filename);

	static Particle* CreateModel(const std::string& filename);

	static std::unique_ptr<Particle> CreateModelPtr(const std::string& filename);

	Microsoft::WRL::ComPtr<ID3D12Resource> GetInstancingResource() { return instancingResource_; }

	void SetColor(const Vector4& color) { materialData_->color = color; }

	ParticleData MakeNewParticle(std::mt19937& randomEngine);

	void ImGuiAdjustParameter();

	void SetWVP(const TransformationMatrix& wvp) { *wvpData_ = wvp; }

private:
	void CreateVertexResource();

	void CreateVertexBufferView();

	void WriteDataToResource();

	void CreateMaterialResource();

	void CreateTransformationMatrixResource();

	void CreateWVPMatrix();

	void SetMaterialData();

	//void ImGuiAdjustParameter();

	void CreateModel();

	void CreateInstance();

	void CreateSRV();

	Matrix4x4 AffineMatrix(const Vector3& scale, const Matrix4x4& rotateMatrix, const Vector3& translate);

	Microsoft::WRL::ComPtr<ID3D12Resource> CreateBufferResource(const Microsoft::WRL::ComPtr<ID3D12Device>& device, size_t sizeInBytes);

	ModelData LoadObjFile(const std::string& directorypath, const std::string& filename);

	ParticleData MakeNewParticle(const Vector3& translate);

	std::list<ParticleData> Emission(const Emitter& emitter);

private: ///メンバ変数///
	// パーティクル
	std::list<ParticleData> particles_{};
	// エミッタ
	Emitter emitter_{};
	// フィールド
	AccField accField_{};

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

	WorldTransform transform_{};
	Matrix4x4 worldMatrix_{};

	WorldTransform cameraTransform_{};

	Matrix4x4 projectionMatrix_{};

	Matrix4x4 cameraMatrix_{};

	Matrix4x4 viewMatrix_{};

	Matrix4x4 worldViewProjectionMatrix_{};

	WorldTransform uvTransform_{};

	Matrix4x4 uvTransformMatrix_{};

	Microsoft::WRL::ComPtr<ID3D12Resource> instancingResource_;

	ParticleForGPU* instancingData_{};

	//ParticleData particles_[MAXINSTANCE]{};

	uint32_t descriptorSizeSRV_{};

	D3D12_CPU_DESCRIPTOR_HANDLE instancingSrvHandleCPU_{};

	D3D12_GPU_DESCRIPTOR_HANDLE instancingSrvHandleGPU_{};

	WorldTransform worldTransform_{};

	uint32_t numInstance_ = 0;

	private: ///メンバ定数///
		float kDeltaTime = 1 / 60.0f;
};

