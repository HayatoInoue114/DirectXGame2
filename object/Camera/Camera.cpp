#include "Camera.h"
#include "../../base/WindowsAPI/WindowsAPI.h"
#include "../../Input/Input.h"

Camera::Camera()
	: transform_({ {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f },{0.0f,0.0f,-30.0f} })
	, FovY_ (0.45f)
	, aspectRatio_(float(kCliantWidth) / float(kCliantHeight))
	, nearClip_(0.1f)
	, farClip_(100.0f)
	, worldMatrix_(MakeAffineMatrix(transform_.scale, transform_.rotate, transform_.translate))
	, viewMatrix_(Inverse(worldMatrix_))
	, projectionMatrix_(MakePerspectiveFovMatrix(FovY_, aspectRatio_, nearClip_, farClip_))
	, viewProjectionMatrix_(Multiply(viewMatrix_, projectionMatrix_))
{}

//Camera::Camera() {
//	Transform_.Initialize();
//	FovY_ = 0.45f;
//	aspectRatio_ = (float(kCliantWidth) / float(kCliantHeight));
//	nearClip_ = 0.1f;
//	farClip_ = 100.0f;
//	worldMatrix_ = MakeAffineMatrix(Transform_.scale, Transform_.rotate, Transform_.translate);
//	viewMatrix_ = Inverse(worldMatrix_);
//	projectionMatrix_ = MakePerspectiveFovMatrix(FovY_, aspectRatio_, nearClip_, farClip_);
//	viewProjectionMatrix_ = Multiply(viewMatrix_, projectionMatrix_);
//}

void Camera::Initialize() {
	transform_.Initialize();
	transform_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f },{0.0f,0.0f,-30.0f} };
	FovY_ = 0.45f;
	aspectRatio_ = (float(kCliantWidth) / float(kCliantHeight));
	nearClip_ = 0.1f;
	farClip_ = 100.0f;
	worldMatrix_ = MakeAffineMatrix(transform_.scale, transform_.rotate, transform_.translate);
	viewMatrix_ = Inverse(worldMatrix_);
	projectionMatrix_ = MakePerspectiveFovMatrix(FovY_, aspectRatio_, nearClip_, farClip_);
	viewProjectionMatrix_ = Multiply(viewMatrix_, projectionMatrix_);
}

void Camera::Update() {
	worldMatrix_ = MakeAffineMatrix(transform_.scale, transform_.rotate, transform_.translate);
	viewMatrix_ = Inverse(worldMatrix_);
	projectionMatrix_ = MakePerspectiveFovMatrix(FovY_, aspectRatio_, nearClip_, farClip_);
	viewProjectionMatrix_ = Multiply(viewMatrix_, projectionMatrix_);

	//カメラの移動スピード
	float speed = 0.2f;
	float accel = 5.0f;

	if (Input::GetInstance()->PushKey(DIK_LSHIFT)) {
		speed = speed * accel;
	}

	//上昇
	if (Input::GetInstance()->PushKey(DIK_SPACE)) {
		transform_.translate.y += speed;
	}
	//下降
	if (Input::GetInstance()->PushKey(DIK_LCONTROL)) {
		transform_.translate.y -= speed;
	}

	//z軸移動
	if (Input::GetInstance()->PushKey(DIK_W)) {
		transform_.translate.z += speed;
	}
	if (Input::GetInstance()->PushKey(DIK_S)) {
		transform_.translate.z -= speed;
	}
	//x軸移動
	if (Input::GetInstance()->PushKey(DIK_D)) {
		transform_.translate.x += speed;
	}
	if (Input::GetInstance()->PushKey(DIK_A)) {
		transform_.translate.x -= speed;
	}

	//Y軸回転
	if (Input::GetInstance()->PushKey(DIK_Q)) {
		transform_.rotate.y -= speed;
	}
	if (Input::GetInstance()->PushKey(DIK_E)) {
		transform_.rotate.y += speed;
	}
	//x軸回転
	if (Input::GetInstance()->PushKey(DIK_I)) {
		transform_.rotate.x += speed;
	}
	if (Input::GetInstance()->PushKey(DIK_K)) {
		transform_.rotate.x -= speed;
	}
	//z軸回転
	if (Input::GetInstance()->PushKey(DIK_L)) {
		transform_.rotate.z += speed;
	}
	if (Input::GetInstance()->PushKey(DIK_J)) {
		transform_.rotate.z -= speed;
	}

	ImGui::Begin("Camera");
	ImGui::DragFloat3("translate", &transform_.translate.x, -0.01f, 0.01f);
	ImGui::DragFloat3("rotate", &transform_.rotate.x, -0.01f, 0.01f);
	ImGui::End();
}