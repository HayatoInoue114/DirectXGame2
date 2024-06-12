#pragma once
//#include <dxcapi.h>
#include "../../base/DirectX12/DirectX12.h"
#include "../../math/Vector4.h"
#include "../../math/MT/MT.h"
#include "../../structure/VertexData/VertexData.h"
#include "../../structure/Material/Material.h"
#include "../../math/WorldTransform/WorldTransform.h"
#include "../../base/GraphicsRenderer/GraphicsRenderer.h"

#pragma comment(lib,"dxcompiler.lib")

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

	void CreateMaterialResource();

	void CreateTransformationMatrixResource();
	
	void Update(WorldTransform& transform,Vector4& color);

	void Draw();

	void CreateWVPMatrix();

	void ImGuiAdjustParameter();
private:
	Material* materialData_{};

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
	VertexData* vertexData{};

	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource_{};

	/*Vector4* materialData_;*/

	Microsoft::WRL::ComPtr<ID3D12Resource> wvpResource_{};
	Matrix4x4* wvpData_{};

	WorldTransform transform_{};
	Matrix4x4 worldMatrix_{};

	WorldTransform cameraTransform_{};
	
	Matrix4x4 projectionMatrix_{};

	Matrix4x4 cameramatrix_{};

	Matrix4x4 viewMatrix_{};

	Matrix4x4 projectionMatix_{};

	Matrix4x4 worldViewProjectionMatrix_{};

	bool useMonsterBall = true;
};


