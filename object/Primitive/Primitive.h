#pragma once
#include "../../../math/WorldTransform/WorldTransform.h"
#include "../Camera/Camera.h"
#include "../../math/TransformationMatrix.h"
#include "../Model/Model.h"
#include "../../../math/MyMath.h"
//#include "../../../base/GraphicsRenderer/GraphicsRenderer.h"

class Primitive
{
public:
	void Initialize();

	void Initialize(const Vector3& pos);

	void Update();

	void Draw();

	void SetCamera(Camera* camera) { this->camera_ = camera; }

	void SetModel(Model* model) { this->model_ = model; }

	/// <summary>
	/// 親となるワールドトランスフォームをセット
	/// </summary>
	/// <param name="model">親となるワールドトランスフォーム</param>
	void SetParent(const WorldTransform* parent);

	void SetPosition(const Vector3& pos) { worldTransform_.translate = pos; }

private:
	WorldTransform worldTransform_;

	TransformationMatrix wvpData_{};

	Camera* camera_ = nullptr;

	//PrimitiveCommon* primitiveCommon_;

	Model* model_ = nullptr;

	bool isParent_ = false;
};

