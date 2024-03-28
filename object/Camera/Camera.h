#pragma once
#include "../../math/Transform/Transform.h"
#include "../../base/DirectX12/DirectX12.h"
#include "../../Input/Input.h"

struct CameraForGPU {
	Vector3 worldPosition;
};

class Camera
{
public:
	Camera();
	void Initialize();
	void Update();
	
	//setter
	void SetRotate(const Vector3& rotate) { Transform_.rotate = rotate; }
	void SetTranslate(const Vector3& translate) { Transform_.translate = translate; }
	void SetFovY(float fov) { FovY_ = fov; }
	void SetAspectRatio(float aspect) { aspectRatio_ = aspect; }
	void SetNearClip(float nearC) { nearClip_ = nearC; }
	void SetFarClip(float farC) { farClip_ = farC; }

	//getter
	const Matrix4x4& GetWorldTransform() const { return worldMatrix_; }
	const Matrix4x4& GetViewMatrix() const { return viewMatrix_; }
	const Matrix4x4& GetProjectionMatrix() const { return projectionMatrix_; }
	const Matrix4x4& GetViewProjectionMatrix() const { return viewProjectionMatrix_; }
	const Vector3& GetRotate() const { return Transform_.rotate; }
	const Vector3& GetTranslate() const { return Transform_.translate; }


private:
	TransformS Transform_;
	Matrix4x4 worldMatrix_;
	Matrix4x4 viewMatrix_;
	Matrix4x4 projectionMatrix_;
	Matrix4x4 viewProjectionMatrix_;

	float FovY_;
	float aspectRatio_;
	float nearClip_;
	float farClip_;

	Microsoft::WRL::ComPtr<ID3D12Resource> cameraForGPUResource;

	CameraForGPU *position;
};

