#include "ParticleManager.h"

void ParticleManager::Initialize() {
	worldTransform_.Initialize();
	//this->camera_ = primitiveCommon_->GetDefaultCamera();
}

void ParticleManager::Update() {
	Matrix4x4 worldMatrix = MakeAffineMatrix(worldTransform_.scale, worldTransform_.rotate, worldTransform_.translate);
	Matrix4x4 worldViewProjectionMatrix{};
	if (camera_) {
		const Matrix4x4& viewProjectionMatrix = camera_->GetViewProjectionMatrix();
		worldViewProjectionMatrix = worldMatrix * viewProjectionMatrix;
	}
	else {
		worldViewProjectionMatrix = worldMatrix;
	}
	wvpData_.WVP = worldViewProjectionMatrix;
	wvpData_.World = worldMatrix;




	//particle_.SetCamera(camera_);
}

void ParticleManager::Draw() {
	//GraphicsRenderer::GetInstance()->SetRootSignatureAndPSO(0);
	//particle_.Draw(CIRCLE);
}