#pragma once
//#include <dxcapi.h>
#include "../../base/DirectX12/DirectX12.h"
#include "../../math/Vector4.h"
#include "../../math/MT/MT.h"
#include "../../structure/VertexData/VertexData.h"
#include "../../structure/Material/Material.h"
#include "../../math/TransformationMatrix.h"

class Light;

class Sphere
{
public:
	void Initialize(DirectX12* directX12, Light* light);

	void Update(Transform& transform, Vector4& color);

	void Draw();

	void CreateVertexResource();

	void CreateVertexBufferView();

	void WriteDataToResource();

	void CreateMaterialResource();

	void CreateTransformationMatrixResource();

	void CreateWVPMatrix();

	void SetMaterialData();

	void ImGuiAdjustParameter();
private:
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
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};

	//頂点リソースにデータを書き込む
	VertexData* vertexData_{};

	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource_{};

	/*Vector4* materialData_;*/

	Microsoft::WRL::ComPtr<ID3D12Resource> wvpResource_{};
	TransformationMatrix* wvpData_{};

	Transform transform_{};
	Matrix4x4 worldMatrix_{};

	Transform cameraTransform_{};

	Matrix4x4 projectionMatrix_{};

	Matrix4x4 cameramatrix_{};

	Matrix4x4 viewMatrix_{};

	Matrix4x4 projectionMatix_{};

	Matrix4x4 worldViewProjectionMatrix_{};

	//分割数
	const uint32_t kSubdivision = 16;
	// 頂点数
	uint32_t vertexIndex_ = kSubdivision * kSubdivision * 6;

	bool useMonsterBall_ = true;

	Transform uvTransform_{};

	Matrix4x4 uvTransformMatrix_{};
};
