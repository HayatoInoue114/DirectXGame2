#pragma once
#include "../../math/MyMath.h"

class Camera
{
public:
	//Camera();
	void Initialize();
	void Update();
	
	//setter
	void SetRotate(const Vector3& rotate) { worldTransform_.rotate = rotate; }
	void SetTranslate(const Vector3& translate) { worldTransform_.translate = translate; }
	void SetFovY(float fov) { FovY_ = fov; }
	void SetAspectRatio(float aspect) { aspectRatio_ = aspect; }
	void SetNearClip(float nearC) { nearClip_ = nearC; }
	void SetFarClip(float farC) { farClip_ = farC; }
	void SetViewMatrix(Matrix4x4 m) { viewMatrix_ = m; }
	void SetProjectionMatrix(Matrix4x4 m) { projectionMatrix_ = m; }

	//getter
	const WorldTransform& GetWorldTransform() const { return worldTransform_; }
	const Matrix4x4& GetViewMatrix() const { return viewMatrix_; }
	const Matrix4x4& GetWorldMatrix() const { return worldMatrix_; }
	const Matrix4x4& GetProjectionMatrix() const { return projectionMatrix_; }
	const Matrix4x4& GetViewProjectionMatrix() const { return viewProjectionMatrix_; }
	const Vector3& GetRotate() const { return worldTransform_.rotate; }
	const Vector3& GetTranslate() const { return worldTransform_.translate; }


private:
	WorldTransform worldTransform_{};
	Matrix4x4 worldMatrix_{};
	Matrix4x4 viewMatrix_{};
	Matrix4x4 projectionMatrix_{};
	Matrix4x4 viewProjectionMatrix_{};

	float FovY_{};
	float aspectRatio_{};
	float nearClip_{};
	float farClip_{};

};

