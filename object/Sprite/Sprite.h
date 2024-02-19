#pragma once
//#include <dxcapi.h>

#include "../../base/DirectX12/DirectX12.h"
#include "../../math/Vector4.h"
#include "../../math/MT/MT.h"
#include "../../structure/VertexData/VertexData.h"
#include "../../structure/Material/Material.h"
#include "../../math/TransformationMatrix.h"

#pragma comment(lib,"dxcompiler.lib")

class Sprite
{
public:
	void Initialize(DirectX12* directX12);

	void Update();

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

	// セッター
	void SetTransform(const Transform& transform) { transform_ = transform; }
	void SetColor(const Vector4& color) { materialData_->color = color; }
	void SetAnchorPoint(const Vector3& anchorpoint) { anchorPoint_ = anchorpoint; }
	void SetisFlipX(bool isFlipX) { isFlipX_ = isFlipX; }
	void SetisFlipY(bool isFlipY) { isFlipY_ = isFlipY; }
	void SetisInvisible(bool isInvisible) { isInvisible_ = isInvisible; }

	// ゲッター
	const Transform& GetTransform() const { return transform_; }
	const Vector4& GetColor() const { return materialData_->color; }
	const Vector3& GetAnchorpoint() const { return anchorPoint_; }
	bool GetisFlipX() const { return isFlipX_; }
	bool GetisFlipY() const { return isFlipY_; }
	bool GetisInvisible() const { return isInvisible_; }

private:
	enum VERTEX {
		LB,
		LT,
		RB,
		RT
	};

	Material* materialData_{};

	DirectX12* directX12_{};

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

	Vector3 anchorPoint_{};

	//左右フリップ
	bool isFlipX_ = false;

	//上下フリップ
	bool isFlipY_ = false;

	//非表示フラグ
	bool isInvisible_ = false;

	Vector2 size_;

	uint32_t textureNum_;
};

