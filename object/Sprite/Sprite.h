#pragma once

#include "../../base/DirectX12/DirectX12.h"
#include "../../math/Vector4.h"
#include "../../math/MT/MT.h"
#include "../../structure/VertexData/VertexData.h"
#include "../../structure/Material/Material.h"
#include "../../math/TransformationMatrix.h"
#include "../../math/WorldTransform/WorldTransform.h"

#pragma comment(lib,"dxcompiler.lib")

class GraphicsRenderer;

class Sprite
{
public:
	void Initialize();

	void Update();

	void Draw();

	void ImGuiAdjustParameter();

	static Sprite* Create(Vector3 position, Vector2 size, Vector4 color, uint32_t textureNum);
	static std::unique_ptr<Sprite> CreateUnique(Vector3 position, Vector2 size, Vector4 color, const std::string& filename);

	// セッター
	void SetWorldTransform(const WorldTransform& transform) { worldTransform_ = transform; }
	void SetColor(const Vector4& color) { materialData_->color = color; }
	void SetAnchorPoint(const Vector3& anchorPoint) { anchorPoint_ = anchorPoint; }
	void SetIsFlipX(bool isFlipX) { isFlipX_ = isFlipX; }
	void SetIsFlipY(bool isFlipY) { isFlipY_ = isFlipY; }
	void SetIsInvisible(bool isInvisible) { isInvisible_ = isInvisible; }
	void SetSize(const Vector2& size) { size_ = size; }
	void SetPosition(const Vector2& position) { worldTransform_.translate.x = position.x; worldTransform_.translate.y = position.y; }
	void SetTextureNum(uint32_t num) { textureNum_ = num; }
	void SetFileName(const std::string& filename) { filename_ = filename; }

	// ゲッター
	const WorldTransform& GetWorldTransform() const { return worldTransform_; }
	const Vector4& GetColor() const { return materialData_->color; }
	const Vector3& GetAnchorPoint() const { return anchorPoint_; }
	bool GetIsFlipX() const { return isFlipX_; }
	bool GetIsFlipY() const { return isFlipY_; }
	bool GetIsInvisible() const { return isInvisible_; }
	Vector2 GetPosition() const { return Vector2(this->worldTransform_.translate.x, this->worldTransform_.translate.y); }

private:
	void CreateVertexResource();

	void CreateVertexBufferView();

	void SetVertexData();

	void CreateTransformationMatrixResource();

	void CalculateAndSetWVPMatrix();

	void CreateMaterialResource();

	void CreateIndex();

	void SetMaterialData();

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

	//WorldTransform transform_{};

	//Matrix4x4 worldMatrix_{};

	Matrix4x4 viewMatrix_{};

	Matrix4x4 projectionMatrix_{};

	Matrix4x4 worldViewProjectionMatrix_{};

	WorldTransform cameraTransform_{};

	Matrix4x4 cameraMatrix_{};

	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource_;

	Microsoft::WRL::ComPtr<ID3D12Resource> indexResource_{};
	
	D3D12_INDEX_BUFFER_VIEW indexBufferView_{};

	uint32_t* indexData_ = nullptr;

	WorldTransform uvTransform_{};

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

	WorldTransform worldTransform_;

	std::string filename_;
};

