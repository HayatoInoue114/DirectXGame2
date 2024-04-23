#pragma once
#include "../Model/Model.h"
#include "../../math/MyMath.h"
#include "../Camera/Camera.h"
#include "../../base/DirectX12/DirectX12.h"

class Object3d
{
public:
	void Init();
	void Init(Model* model);
	void Init(Camera* camera);
	void Init(Model* model, Camera* camera);

	void Draw();


public:
	void SetModel(Model* model) { this->model_ = model; }
	void SetParent(const WorldTransform* parent);
	void SetCamera(Camera* camera) { camera_ = camera; }

	void SetWorldTransform(const WorldTransform& world) { worldTransform_ = world; }
	void SetRotate(const Vector3& rotate) { worldTransform_.rotate = rotate; }
	void SetTranslate(const Vector3& translate) { worldTransform_.translate = translate; }
private:
	void CreateWVPMatrix();

	void CreateTransformationMatrixResource();
public:
	bool isParent_ = false;

private:
	Model* model_ = nullptr;

	WorldTransform worldTransform_{};

	ID3D12Resource* wvpResource_{};

	TransformationMatrix* wvpData_{};

	Camera* camera_{};

	Matrix4x4 worldMatrix_{};

	Matrix4x4 worldViewProjectionMatrix_{};
};

