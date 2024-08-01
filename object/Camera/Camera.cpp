#include "Camera.h"
#include "../../base/WindowsAPI/WindowsAPI.h"
#include "../../Input/Input.h"
#include "../../base/GraphicsRenderer/GraphicsRenderer.h"
//Camera::Camera()
//	: worldTransform_({ {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f },{0.0f,0.0f,-30.0f} })
//	, FovY_ (0.45f)
//	, aspectRatio_(float(kCliantWidth) / float(kCliantHeight))
//	, nearClip_(0.1f)
//	, farClip_(100.0f)
//	, worldMatrix_(MakeAffineMatrix(worldTransform_.scale, worldTransform_.rotate, worldTransform_.translate))
//	, viewMatrix_(Inverse(worldMatrix_))
//	, projectionMatrix_(MakePerspectiveFovMatrix(FovY_, aspectRatio_, nearClip_, farClip_))
//	, viewProjectionMatrix_(Multiply(viewMatrix_, projectionMatrix_))
//{}

Camera::Camera() {
	worldTransform_.Initialize();
	FovY_ = 0.45f;
	aspectRatio_ = (float(kCliantWidth) / float(kCliantHeight));
	nearClip_ = 0.1f;
	farClip_ = 100.0f;
	worldMatrix_ = MakeAffineMatrix(worldTransform_.scale, worldTransform_.rotate, worldTransform_.translate);
	viewMatrix_ = Inverse(worldMatrix_);
	projectionMatrix_ = MakePerspectiveFovMatrix(FovY_, aspectRatio_, nearClip_, farClip_);
	viewProjectionMatrix_ = Multiply(viewMatrix_, projectionMatrix_);
	cameraForGPUResource_ = DirectX12::GetInstance()->CreateBufferResource(sizeof(CameraForGPU));
	position = nullptr;
	cameraForGPUResource_->Map(0, nullptr, reinterpret_cast<void**>(&position));
}

void Camera::Initialize() {
	worldTransform_.Initialize();
	worldTransform_.scale = {1,1,1};
	worldTransform_.rotate = {0,0,0};
	worldTransform_.translate = { 0.0f,0.0f,-30.0f };
	FovY_ = 0.45f;
	aspectRatio_ = (float(kCliantWidth) / float(kCliantHeight));
	nearClip_ = 0.1f;
	farClip_ = 1000.0f;
	worldMatrix_ = MakeAffineMatrix(worldTransform_.scale, worldTransform_.rotate, worldTransform_.translate);
	viewMatrix_ = Inverse(worldMatrix_);
	projectionMatrix_ = MakePerspectiveFovMatrix(FovY_, aspectRatio_, nearClip_, farClip_);
	viewProjectionMatrix_ = Multiply(viewMatrix_, projectionMatrix_);
	cameraForGPUResource_ = DirectX12::GetInstance()->CreateBufferResource(sizeof(CameraForGPU));
	position = nullptr;
	cameraForGPUResource_->Map(0, nullptr, reinterpret_cast<void**>(&position));
}

void Camera::Update() {
	worldMatrix_ = MakeAffineMatrix(worldTransform_.scale, worldTransform_.rotate, worldTransform_.translate);
	viewMatrix_ = Inverse(worldMatrix_);
	projectionMatrix_ = MakePerspectiveFovMatrix(FovY_, aspectRatio_, nearClip_, farClip_);
	viewProjectionMatrix_ = Multiply(viewMatrix_, projectionMatrix_);

#pragma region カメラのキー移動
	//カメラの移動スピード
	float speed = 0.2f;
	//カメラの回転速度
	float rotateSpeed = 0.02f;
	//ダッシュ
	float accel = 5.0f;
	//後でtranslateに入れる値
	Vector3 move{};

	if (Input::GetInstance()->PushKey(DIK_LSHIFT)) {
		speed = speed * accel;
	}

	//上昇
	if (Input::GetInstance()->PushKey(DIK_SPACE)) {
		worldTransform_.translate.y += speed;
	}
	//下降
	if (Input::GetInstance()->PushKey(DIK_LCONTROL)) {
		worldTransform_.translate.y -= speed;
	}

	//z軸移動
	if (Input::GetInstance()->PushKey(DIK_W)) {
		move.z = speed;
	}
	if (Input::GetInstance()->PushKey(DIK_S)) {
		move.z = -speed;
	}
	//x軸移動
	if (Input::GetInstance()->PushKey(DIK_D)) {
		move.x = speed;
	}
	if (Input::GetInstance()->PushKey(DIK_A)) {
		move.x = -speed;
	}

	//Y軸回転
	if (Input::GetInstance()->PushKey(DIK_Q) || Input::GetInstance()->PushKey(DIK_LEFT)) {
		worldTransform_.rotate.y -= rotateSpeed;
	}
	if (Input::GetInstance()->PushKey(DIK_E) || Input::GetInstance()->PushKey(DIK_RIGHT)) {
		worldTransform_.rotate.y += rotateSpeed;
	}
	//x軸回転
	if (Input::GetInstance()->PushKey(DIK_I) || Input::GetInstance()->PushKey(DIK_UP)) {
		worldTransform_.rotate.x -= rotateSpeed;
	}
	if (Input::GetInstance()->PushKey(DIK_K) || Input::GetInstance()->PushKey(DIK_DOWN)) {
		worldTransform_.rotate.x += rotateSpeed;
	}
	//z軸回転
	if (Input::GetInstance()->PushKey(DIK_L)) {
		worldTransform_.rotate.z += rotateSpeed;
	}
	if (Input::GetInstance()->PushKey(DIK_J)) {
		worldTransform_.rotate.z -= rotateSpeed;
	}

	//向いてる方向へ移動するように
	Matrix4x4 wM = MakeRotateXYZMatrix(worldTransform_.rotate);
	move = {
		move.x * wM.m[0][0] + move.z * wM.m[2][0],
		move.x * wM.m[0][1] + move.z * wM.m[2][1],
		move.x * wM.m[0][2] + move.z * wM.m[2][2]
	};

	worldTransform_.translate += move;

#pragma endregion カメラのキー移動

	/*GraphicsRenderer::GetInstance()->SetRootSignatureAndPSO(GraphicsRenderer::Object3d);
	DirectX12::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(4, cameraForGPUResource_->GetGPUVirtualAddress());

	GraphicsRenderer::GetInstance()->SetRootSignatureAndPSO(GraphicsRenderer::Skinning);
	DirectX12::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(4, cameraForGPUResource_->GetGPUVirtualAddress());*/

	ImGui::Begin("Camera");
	ImGui::DragFloat3("translate", &worldTransform_.translate.x, -0.01f, 0.01f);
	ImGui::DragFloat3("rotate", &worldTransform_.rotate.x, -0.01f, 0.01f);
	ImGui::End();

	
}