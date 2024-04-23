#include "Object3d.h"

void Object3d::Init()
{
	CreateTransformationMatrixResource();
}

void Object3d::Init(Model* model)
{
	model_ = model;
	CreateTransformationMatrixResource();
}

void Object3d::Init(Camera* camera)
{
	camera_ = camera;
	CreateTransformationMatrixResource();
}

void Object3d::Init(Model* model,Camera* camera)
{
	camera_ = camera;
	model_ = model;
	CreateTransformationMatrixResource();
}

void Object3d::Draw()
{
	CreateWVPMatrix();

	//wvp用のCBufferの場所を設定
	DirectX12::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(1, wvpResource_->GetGPUVirtualAddress());
	DirectX12::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(3, Light::Getinstance()->GetDirectionalLightResource()->GetGPUVirtualAddress());


	if (model_) {
		model_->Draw();
	}

}

void Object3d::SetParent(const WorldTransform* parent)
{
	worldTransform_.parent_ = parent;
	isParent_ = true;
}

void Object3d::CreateWVPMatrix()
{
	Matrix4x4 worldMatrix = MakeAffineMatrix(worldTransform_.scale, worldTransform_.rotate, worldTransform_.translate);
	Matrix4x4 worldViewProjectionMatrix{};
	if (camera_) {
		if (isParent_) {
			worldMatrix = Multiply(worldMatrix, camera_->GetWorldMatrix());
		}
		const Matrix4x4& viewProjectionMatrix = camera_->GetViewProjectionMatrix();
		worldViewProjectionMatrix = Multiply(worldMatrix, viewProjectionMatrix);
	}
	else {
		worldViewProjectionMatrix = worldMatrix;
	}
	wvpData_->WVP = worldViewProjectionMatrix;
	wvpData_->World = worldMatrix;
}

void Object3d::CreateTransformationMatrixResource() {
	//WVP用のリソースを作る。Matrix4x4　1つ分のサイズを用意する
	wvpResource_ = DirectX12::GetInstance()->CreateBufferResource(DirectX12::GetInstance()->GetDevice().Get(), sizeof(TransformationMatrix));
	//データを書き込む
	wvpData_ = nullptr;
	//書き込むためのアドレスを取得
	wvpResource_->Map(0, nullptr, reinterpret_cast<void**>(&wvpData_));
	//単位行列を書き込んでおく
	wvpData_->WVP = MakeIdentity4x4();
	wvpData_->World = MakeIdentity4x4();
}