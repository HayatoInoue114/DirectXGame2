#pragma once
#include "../../../math/WorldTransform/WorldTransform.h"
#include "../../Camera/Camera.h"
#include "../../../math/TransformationMatrix.h"
#include "../PrimitiveCommon/PrimitiveCommon.h"
#include "../../Model/Model.h"
#include "../../../math/MT/MT.h"
//#include "../../../base/GraphicsRenderer/GraphicsRenderer.h"

class Primitive
{
public:
	void Initialize();

	void Update();

	void Draw();

	void SetCamera(Camera* camera) { this->camera_ = camera; }

	void SetModel(Model* model) { this->model_ = model; }

private:
	WorldTransform worldTransform_;

	TransformationMatrix wvpData_{};

	Camera* camera_ = nullptr;

	//PrimitiveCommon* primitiveCommon_;

	Model* model_ = nullptr;
};

