#pragma once
//#include <dxcapi.h>

#include "../../base/DirectX12/DirectX12.h"
#include "../../math/Vector4.h"
#include "../../math/MT/MT.h"
#include "../../structure/VertexData/VertexData.h"
#include "../../structure/Material/Material.h"
#include "../../math/WorldTransform/WorldTransform.h"
#include "../../manager/TextureManager/TextureManager.h"

#pragma comment(lib,"dxcompiler.lib")

class Sprite
{
public:
	Sprite();
	~Sprite();

	void Initialize();

	void Update();

	void Draw(WorldTransform& worldTransform);

	void CreateVertexResource();

	void CreateVertexBufferView();

	void SetVertexData();

	void CreateTransformationMatrixResource();

	void CalculateAndSetWVPMatrix();

	void CreateMaterialResource();

	void CreateIndex();

	void SetMaterialData();

	void ImGuiAdjustParameter();

	static Sprite* Create(Vector3 position, Vector2 size, Vector4 color, uint32_t textureNum);

	// セッター
	void SetWorldTransform(const WorldTransform& transform) { worldTransform_ = transform; }
	void SetColor(const Vector4& color) { materialData_->color = color; }
	void SetAnchorPoint(const Vector3& anchorpoint) { anchorPoint_ = anchorpoint; }
	void SetisFlipX(bool isFlipX) { isFlipX_ = isFlipX; }
	void SetisFlipY(bool isFlipY) { isFlipY_ = isFlipY; }
	void SetisInvisible(bool isInvisible) { isInvisible_ = isInvisible; }
	void SetSize(const Vector2& size) { size_ = size; }
	void SetPosition(const Vector2& position) { worldTransform_.translation_.x = position.x; worldTransform_.translation_.y = position.y; }
	void SetTextureNum(uint32_t num) { textureNum_ = num; }

	// ゲッター
	const WorldTransform& GetWorldTransform() const { return worldTransform_; }
	const Vector4& GetColor() const { return materialData_->color; }
	const Vector3& GetAnchorpoint() const { return anchorPoint_; }
	bool GetisFlipX() const { return isFlipX_; }
	bool GetisFlipY() const { return isFlipY_; }
	bool GetisInvisible() const { return isInvisible_; }
	const Vector2& GetPosition() const { return { worldTransform_.translation_.x ,worldTransform_.translation_.y }; }
	

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

	//Transform transform_{};

	Matrix4x4 worldMatrix_{};

	Matrix4x4 viewMatrix_{};

	Matrix4x4 projectionMatrix_{};

	Matrix4x4 worldViewprojectionMatrix_{};

	WorldTransform cameraTransform_{};

	Matrix4x4 cameramatrix_{};

	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource_;

	Microsoft::WRL::ComPtr<ID3D12Resource> indexResource_{};
	
	D3D12_INDEX_BUFFER_VIEW indexBufferView_{};

	uint32_t* indexData_ = nullptr;

	WorldTransform uvTransform_{};

	Matrix4x4 uvTransformMatrix_{};

	Vector3 anchorPoint_{ 0.5f,0.5f };

	//左右フリップ
	bool isFlipX_ = false;

	//上下フリップ
	bool isFlipY_ = false;

	//非表示フラグ
	bool isInvisible_ = false;

	Vector2 size_;

	WorldTransform worldTransform_;

	uint32_t textureNum_;
};

