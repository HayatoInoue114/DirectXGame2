#include "Camera.h"
#include "../../base/WindowsAPI/WindowsAPI.h"
#include "../../Input/Input.h"

//Camera::Camera()
//	: transform_({ {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f },{0.0f,0.0f,-30.0f} })
//	, FovY_ (0.45f)
//	, aspectRatio_(float(kCliantWidth) / float(kCliantHeight))
//	, nearClip_(0.1f)
//	, farClip_(100.0f)
//	, worldMatrix_(MakeAffineMatrix(transform_.scale, transform_.rotate, transform_.translate))
//	, viewMatrix_(Inverse(worldMatrix_))
//	, projectionMatrix_(MakePerspectiveFovMatrix(FovY_, aspectRatio_, nearClip_, farClip_))
//	, viewProjectionMatrix_(Multiply(viewMatrix_, projectionMatrix_))
//{}

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
		move.y = speed;
	}
	//下降
	if (Input::GetInstance()->PushKey(DIK_LCONTROL)) {
		move.y = -speed;
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
	if (Input::GetInstance()->PushKey(DIK_Q)) {
		transform_.rotate.y -= rotateSpeed;
	}
	if (Input::GetInstance()->PushKey(DIK_E)) {
		transform_.rotate.y += rotateSpeed;
	}
	//x軸回転
	if (Input::GetInstance()->PushKey(DIK_I)) {
		transform_.rotate.x -= rotateSpeed;
	}
	if (Input::GetInstance()->PushKey(DIK_K)) {
		transform_.rotate.x += rotateSpeed;
	}
	//z軸回転
	if (Input::GetInstance()->PushKey(DIK_L)) {
		transform_.rotate.z += rotateSpeed;
	}
	if (Input::GetInstance()->PushKey(DIK_J)) {
		transform_.rotate.z -= rotateSpeed;
	}

	//向いてる方向へ移動するように
	Matrix4x4 wM = MakeRotateXYZMatrix(transform_.rotate);
	move = {
		move.x * wM.m[0][0] + move.z * wM.m[2][0],
		move.x * wM.m[0][1] + move.z * wM.m[2][1],
		move.x * wM.m[0][2] + move.z * wM.m[2][2]
	};

	transform_.translate += move;

	ImGui::Begin("Camera");
	ImGui::DragFloat3("translate", &transform_.translate.x, -0.01f, 0.01f);
	ImGui::DragFloat3("rotate", &transform_.rotate.x, -0.01f, 0.01f);
	ImGui::End();

	
}