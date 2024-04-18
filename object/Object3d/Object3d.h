#pragma once
#include "../Model/Model.h"
#include "../../math/MyMath.h"
#include "../Camera/Camera.h"
#include "../../base/DirectX12/DirectX12.h"
#include "../../base/GraphicsRenderer/GraphicsRenderer.h"

class Object3d
{
public:
	void Init();

	void Draw();


public:
	void SetModel(Model* model) { this->model = model; }

	void SetParent(const WorldTransform* parent);

	void SetCamera(Camera* camera) { camera_ = camera; }
private:
	void CreateWVPMatrix();

	void CreateTransformationMatrixResource();
public:
	bool isParent_ = false;

private:
	Model* model = nullptr;

	WorldTransform worldTransform_{};

	ID3D12Resource* wvpResource_{};

	TransformationMatrix* wvpData_{};

	Camera* camera_{};
};

