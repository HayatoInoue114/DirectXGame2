#include "Camera.h"
#include "../../base/WindowsAPI/WindowsAPI.h"


Camera::Camera()
	: Transform_({ {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f },{0.0f,0.0f,-30.0f} })
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
	Transform_ = { {1.0f,1.0f,1.0f},{-0.3f,0.0f,0.0f },{0.0f,-9.0f,-30.0f} };
	FovY_ = 0.45f;
	aspectRatio_ = (float(kCliantWidth) / float(kCliantHeight));
	nearClip_ = 0.1f;
	farClip_ = 100.0f;
	worldMatrix_ = MakeAffineMatrix(Transform_.scale, Transform_.rotate, Transform_.translate);
	viewMatrix_ = Inverse(worldMatrix_);
	projectionMatrix_ = MakePerspectiveFovMatrix(FovY_, aspectRatio_, nearClip_, farClip_);
	viewProjectionMatrix_ = Multiply(viewMatrix_, projectionMatrix_);

	/*cameraForGPUResource = DirectX12::GetInstance()->CreateBufferResource(DirectX12::GetInstance()->GetDevice().Get(), sizeof(CameraForGPU));
	position = nullptr;
	cameraForGPUResource->Map(0, nullptr, reinterpret_cast<void**>(&position));*/
}

void Camera::Update() {
	worldMatrix_ = MakeAffineMatrix(Transform_.scale, Transform_.rotate, Transform_.translate);
	viewMatrix_ = Inverse(worldMatrix_);
	projectionMatrix_ = MakePerspectiveFovMatrix(FovY_, aspectRatio_, nearClip_, farClip_);
	viewProjectionMatrix_ = Multiply(viewMatrix_, projectionMatrix_);

	//DirectX12::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(4, cameraForGPUResource->GetGPUVirtualAddress());

	//if (Input::GetInstance()->PushKey(DIK_UP)) {
	//	Transform_.translate.x
	//}
	

	ImGui::Begin("Camera");
	ImGui::DragFloat3("translate", &Transform_.translate.x, -0.1, 0.1);
	ImGui::DragFloat3("rotate", &Transform_.rotate.x, -0.1, 0.1);
	ImGui::End();
}