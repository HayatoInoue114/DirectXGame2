#include "Skydome.h"
#include "../../object/Model/Model.h"

void Skydome::Initialize(Model *model) {
	model_ = model;
	worldTransform_.Initialize();
}
void Skydome::Initialize(Model* model,Camera* camera) {
	model_ = model;
	worldTransform_.Initialize();
	camera_ = camera;
}

void Skydome::Update() {}

void Skydome::Draw() { 
	Matrix4x4 worldMatrix = MakeAffineMatrix(worldTransform_.scale, worldTransform_.rotate, worldTransform_.translate);
	Matrix4x4 worldViewProjectionMatrix{};
	if (camera_) {
		const Matrix4x4& viewProjectionMatrix = camera_->GetViewProjectionMatrix();
		worldViewProjectionMatrix = worldMatrix * viewProjectionMatrix;
	}
	else {
		worldViewProjectionMatrix = worldMatrix;
	}

	TransformationMatrix wvpData{};
	wvpData.WVP = worldViewProjectionMatrix;
	wvpData.World = worldMatrix;

	model_->SetWVP(wvpData);
	model_->Draw();
}