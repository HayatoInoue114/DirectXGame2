#include "Primitive.h"
#include "../../../base/GraphicsRenderer/GraphicsRenderer.h"

void Primitive::Initialize() {
	worldTransform_.Initialize();

	Scope scope = { -10.0f,10.0f };
	ScopeVec3 scope3 = { scope,scope,scope };
	//worldTransform_.translate = RandomGenerator::GetInstance()->getRandom(scope3);
	//this->camera_ = primitiveCommon_->GetDefaultCamera();
}
void Primitive::Initialize(const Vector3& pos) {
	worldTransform_.Initialize();
	worldTransform_.translate = pos;

	Scope scope = { -10.0f,10.0f };
	ScopeVec3 scope3 = { scope,scope,scope };

	
	//worldTransform_.translate = RandomGenerator::GetInstance()->getRandom(scope3);
	//this->camera_ = primitiveCommon_->GetDefaultCamera();
}

void Primitive::Update() {
	Matrix4x4 worldMatrix = MakeAffineMatrix(worldTransform_.scale, worldTransform_.rotate, worldTransform_.translate);
	Matrix4x4 worldViewProjectionMatrix{};
	if (camera_) {
		if (isParent_) {
			worldMatrix = Multiply(worldMatrix,camera_->GetWorldMatrix());
		}
		const Matrix4x4& viewProjectionMatrix = camera_->GetViewProjectionMatrix();
		worldViewProjectionMatrix = Multiply(worldMatrix,viewProjectionMatrix);
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

void Primitive::SetParent(const WorldTransform* parent)
{
	worldTransform_.parent_ = parent;
	isParent_ = true;
}
