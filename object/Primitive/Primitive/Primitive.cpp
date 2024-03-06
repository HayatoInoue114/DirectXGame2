#include "Primitive.h"

void Primitive::Initialize() {
	worldTransform_.Initialize();
	//this->camera_ = primitiveCommon_->GetDefaultCamera();
}

void Primitive::Update() {
	Matrix4x4 worldMatrix = MakeAffineMatrix(worldTransform_.scale, worldTransform_.rotate, worldTransform_.translate);
	Matrix4x4 worldViewProjectionMatrix;
	if (camera_) {
		const Matrix4x4& viewProjectionMatrix = camera_->GetViewProjectionMatrix();
		worldViewProjectionMatrix = worldMatrix * viewProjectionMatrix;
	}
	else {
		worldViewProjectionMatrix = worldMatrix;
	}
	wvpData_.WVP = worldViewProjectionMatrix;
	wvpData_.World = worldMatrix;
}

void Primitive::Draw() {
	//GraphicsRenderer::GetInstance()->SetRootSignatureAndPSO(0);
	if (model_) {
		model_->SetWVP(wvpData_);
		model_->Draw();
	}
}