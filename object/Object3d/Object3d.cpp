#include "Object3d.h"
#include "../../../base/GraphicsRenderer/GraphicsRenderer.h"
#include "../../Input/Input.h"

void Object3d::Init()
{
	CreateTransformationMatrixResource();
}

void Object3d::Init(const std::string& filename)
{
	std::unique_ptr<Model> model = std::make_unique<Model>();
	model = Model::CreateModelPtr(filename);
	model_ = model.get();
	CreateTransformationMatrixResource();
}

void Object3d::Init(const std::string& filename, Camera* camera)
{
	std::unique_ptr<Model> model;
	model = Model::CreateModelPtr(filename);
	model_ = model.get();
	camera_ = camera;
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

	if (model_) {
		//もしアニメーションを使うならSkinningShaderを使う(それ以外ならObject3d)
		
		/*if (skeleton_) {
			DrawJoint(*skeleton_, camera_);
		}*/

		if (animation_) {
			GraphicsRenderer::GetInstance()->SetRootSignatureAndPSO(GraphicsRenderer::Skinning);
		}
		else {
			GraphicsRenderer::GetInstance()->SetRootSignatureAndPSO(GraphicsRenderer::Object3d);
		}

		if (!model_->isObj) {
			wvpData_->WVP = worldViewProjectionMatrix_;
			wvpData_->World = worldMatrix_;

		}

		

		D3D12_VERTEX_BUFFER_VIEW vbvs[2] = {
			model_->GetVBV(),
			skinCluster_.influenceBufferView
		};
		//wvp用のCBufferの場所を設定
		DirectX12::GetInstance()->GetCommandList()->IASetVertexBuffers(0, 2, vbvs);
		DirectX12::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(1, wvpResource_->GetGPUVirtualAddress());
		DirectX12::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(3, Light::Getinstance()->GetDirectionalLightResource()->GetGPUVirtualAddress());
		DirectX12::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(4, camera_->GetGPUResource()->GetGPUVirtualAddress());
		if (animation_) {
			DirectX12::GetInstance()->GetCommandList()->SetGraphicsRootDescriptorTable(6, skinCluster_.paletteSrvHandle.second);
			//SrvManager::GetInstance()->SetGraphicsRootDescriptorTable(4, srvIndex_);
		}
		Light::Getinstance()->AdjustParameter();
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
	worldMatrix_ = MakeAffineMatrix(worldTransform_.scale, worldTransform_.rotate, worldTransform_.translate);
	if (camera_) {
		if (isParent_) {
			worldMatrix_ = Multiply(worldMatrix_, camera_->GetWorldMatrix());
		}
		const Matrix4x4& viewProjectionMatrix = camera_->GetViewProjectionMatrix();
		worldViewProjectionMatrix_ = Multiply(worldMatrix_, viewProjectionMatrix);
	}
	else {
		worldViewProjectionMatrix_ = worldMatrix_;
	}

	wvpData_->WVP = worldViewProjectionMatrix_;
	wvpData_->World = worldMatrix_;
}

void Object3d::CreateTransformationMatrixResource() 
{
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

void Object3d::LoadAnimation(const std::string& filename) 
{
	//インスタンス生成
	if (!animation_) {
		animation_ = std::make_unique<Animation>();
	}

	*animation_ = LoadAnimationFile(filename);

	if (!skeleton_) {
		skeleton_ = std::make_unique<Skeleton>();
	}

	*skeleton_ = CreateSkeleton(model_->GetModelData().rootNode);
	//CreateResourceView();
	

	skinCluster_ = CreateSkinCluster(DirectX12::GetInstance()->GetDevice(), *skeleton_, model_->GetModelData());
}

void Object3d::UpdateAnimation()
{
	// キャラクターの移動ベクトル
	Vector3 move = { 0, 0, 0 };

	Vector3 rotate = {};

	// キャラクターの移動の速さ
	const float kCharacterSpeed = 0.2f;

	//ゲームパッドの状態を得る変数（XINPUT）
	XINPUT_STATE joyState;

	//ゲームパッド状態取得
	if (Input::GetInstance()->GetJoystickState(joyState)) {
		move.x += (float)joyState.Gamepad.sThumbLX / SHRT_MAX * kCharacterSpeed;
		move.z += (float)joyState.Gamepad.sThumbLY / SHRT_MAX * kCharacterSpeed;
	}

	worldTransform_.translate += move;

	isEndAnimation_ = false;

	//アニメーションが存在していて、再生フラグが立っている時
	if (animation_ && isStartAnimation_ && animation_->nodeAnimations.size() != 0) {

		//現在のアニメーションタイムをアニメーション速度分加算
		animationTime_ += animationSpeed_ / 60.0f;

		//アニメーションタイムが全体の尺を超えていたら終点とみなす
		if (animationTime_ >= animation_->duration) {

			animationTime_ = animation_->duration;

			//ループしなければフラグを降ろす
			if (!isLoop_) {
				isStartAnimation_ = false;
			}

			isEndAnimation_ = true;
		}

		//アニメーションの時間調整
		animationTime_ = std::fmod(animationTime_, animation_->duration);

		if (skeleton_) {
			ApplyAnimation(*skeleton_, *animation_, animationTime_);
			UpdateSkeleton(*skeleton_);
			UpdateSkinCluster(skinCluster_, *skeleton_);
		}

		//NodeAnimation& rootNodeAnimation = animation_->nodeAnimations[model_->GetModelData().rootNode.name]; //rootNodeのanimationを取得
		//Vector3 translate = CalculateValue(rootNodeAnimation.translate.keyFrames, animationTime_);
		//Quaternion rotate = CalculateValue(rootNodeAnimation.rotate.keyFrames, animationTime_);
		//Vector3 scale = CalculateValue(rootNodeAnimation.scale.keyFrames, animationTime_);
		//localMatrix_ = MakeAffineMatrix(scale, rotate, translate);
	}
}

void Object3d::ResetAnimation() 
{
	//アニメーションが存在している時のみリセットを行う
	if (animation_ && animation_->nodeAnimations.size() != 0) {
		animationTime_ = 0.0f;
		NodeAnimation& rootNodeAnimation = animation_->nodeAnimations[model_->GetModelData().rootNode.name]; //rootNodeのanimationを取得
		Vector3 translate = CalculateValue(rootNodeAnimation.translate.keyFrames, animationTime_);
		Quaternion rotate = CalculateValue(rootNodeAnimation.rotate.keyFrames, animationTime_);
		Vector3 scale = CalculateValue(rootNodeAnimation.scale.keyFrames, animationTime_);
		localMatrix_ = MakeAffineMatrix(scale, rotate, translate);
	}
}


