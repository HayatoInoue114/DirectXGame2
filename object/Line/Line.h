//#pragma once
//#include <string>
//#include <fstream>
//#include <sstream>
//
//
//#include "../../base/DirectX12/DirectX12.h"
//#include "../../math/MyMath.h"
//#include "../../structure/VertexData/VertexData.h"
//#include "../../structure/Material/Material.h"
//#include "../../manager/TextureManager/TextureManager.h"
//#include "../../structure/ModelData/ModelData.h"
//#include "../../math/WorldTransform/WorldTransform.h"
//#include "../../manager/ModelManager/ModelManager.h"
//#include "../../math/Transform/Transform.h"
//#include "../Camera/Camera.h"
//
//class Line
//{
//public:
//	Line();
//	~Line();
//
//	static const int32_t kVertexNum_ = 2;
//
//	static void Initialize(ID3D12Device* device);
//
//	static void PreDraw(ID3D12GraphicsCommandList* commandList);
//
//	static void PostDraw();
//
//	void Draw(Camera* camera, const Matrix4x4& matrix);
//
//	Vector3 start_;
//
//	Vector3 end_;
//
//	Vector4 color_;
//
//private:
//
//	//デバイス
//	static ID3D12Device* device_;
//	//コマンドリスト
//	static ID3D12GraphicsCommandList* commandList_;
//	//ルートシグネチャ
//	static ID3D12RootSignature* rootSignature_;
//	//PSO
//	static ID3D12PipelineState* pipelineState_;
//
//private:
//
//	struct LineVertexData {
//		Vector4 position;
//		Vector4 color;
//	};
//
//	//頂点リソース
//	Microsoft::WRL::ComPtr<ID3D12Resource> vertexBuff_;
//
//	D3D12_VERTEX_BUFFER_VIEW vbView_{};
//
//	//ビュープロジェクション行列リソース
//	Microsoft::WRL::ComPtr<ID3D12Resource> wvpBuff_;
//
//	Matrix4x4* wvpMatrix_ = nullptr;
//
//	LineVertexData* vertexMap_ = nullptr;
//};
//
