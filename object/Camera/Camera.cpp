#include "Camera.h"
#include "../../base/WindowsAPI/WindowsAPI.h"

Camera::Camera()
	: Transform_({ {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f },{0.0f,0.0f,0.0f} })
	, FovY_ (0.45f)
	, aspectRatio_(float(kCliantWidth) / float(kCliantHeight))
	, nearClip_(0.1f)
	, farClip_(100.0f)
	, worldMatrix_(MakeAffineMatrix(Transform_.scale, Transform_.rotate, Transform_.translate))
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
	Transform_.Initialize();
	FovY_ = 0.45f;
	aspectRatio_ = (float(kCliantWidth) / float(kCliantHeight));
	nearClip_ = 0.1f;
	farClip_ = 100.0f;
	worldMatrix_ = MakeAffineMatrix(Transform_.scale, Transform_.rotate, Transform_.translate);
	viewMatrix_ = Inverse(worldMatrix_);
	projectionMatrix_ = MakePerspectiveFovMatrix(FovY_, aspectRatio_, nearClip_, farClip_);
	viewProjectionMatrix_ = Multiply(viewMatrix_, projectionMatrix_);
}

void Camera::Update() {
	worldMatrix_ = MakeAffineMatrix(Transform_.scale, Transform_.rotate, Transform_.translate);
	viewMatrix_ = Inverse(worldMatrix_);
	projectionMatrix_ = MakePerspectiveFovMatrix(FovY_, aspectRatio_, nearClip_, farClip_);
	viewProjectionMatrix_ = Multiply(viewMatrix_, projectionMatrix_);

	ImGui::Begin("Camera");
	ImGui::SliderFloat3("translate", &Transform_.translate.x, -10, 10);
	ImGui::SliderFloat3("rotate", &Transform_.rotate.x, -10, 10);
	ImGui::End();
}