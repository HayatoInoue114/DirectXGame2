#pragma once
#include "../Model/Model.h"
#include "../../math/MyMath.h"
#include "../Camera/Camera.h"
#include "../../base/DirectX12/DirectX12.h"
#include "Animation/Animation.h"
#include "../../utility/UtilityFunction/UtilityFunction.h"

class GraphicsRenderer;

class Object3d
{
public:
	void Init();
	void Init(Model* model);
	void Init(Camera* camera);
	void Init(Model* model, Camera* camera);

	void Draw();

	void LoadAnimation(const std::string& filename);
	void CreateResourceView();
	void UpdateAnimation();
	void ResetAnimation();
public:
	void SetModel(Model* model) { this->model_ = model; }
	void SetParent(const WorldTransform* parent);
	void SetCamera(Camera* camera) { camera_ = camera; }

	void SetWorldTransform(const WorldTransform& world) { worldTransform_ = world; }
	void SetRotate(const Vector3& rotate) { worldTransform_.rotate = rotate; }
	void SetTranslate(const Vector3& translate) { worldTransform_.translate = translate; }

	void SetSkeleton(const Skeleton& skeleton) { *skeleton_ = skeleton; }

	/// <summary>
	/// アニメーション開始
	/// </summary>
	/// <param name="isLoop">ループするかどうか</param>
	void StartAnimation(bool isLoop = false) {
		isStartAnimation_ = true;
		isLoop_ = isLoop;
	}
	//アニメーション一時停止
	void StopAnimation() { isStartAnimation_ = false; }
	//アニメーションのループフラグを設定
	void SetIsLoop(bool flag) { isLoop_ = flag; }
	//アニメーション速度変更
	void SetAnimationSpeed(float speed) { animationSpeed_ = speed; }

	Skeleton* GetSkelton() { return skeleton_.get(); }
private:
	void CreateWVPMatrix();

	void CreateTransformationMatrixResource();
public:
	bool isParent_ = false;

private:
	Model* model_ = nullptr;

	WorldTransform worldTransform_{};

	ID3D12Resource* wvpResource_{};

	TransformationMatrix* wvpData_{};

	Camera* camera_{};

	Matrix4x4 worldMatrix_{};

	Matrix4x4 worldViewProjectionMatrix_{};

	//アニメーション
	std::unique_ptr<Animation> animation_;

	//Skeleton
	std::unique_ptr<Skeleton> skeleton_;

	//アニメーションタイム
	float animationTime_ = 0.0f;

	//アニメーション速度
	float animationSpeed_ = 1.0f;

	//アニメーション管理フラグ
	bool isStartAnimation_ = false;

	//ループ管理フラグ
	bool isLoop_ = false;

	//アニメーションの終点に到達したかどうか
	bool isEndAnimation_ = false;

	//アニメーションのマトリックス
	Matrix4x4 localMatrix_{};

	SkinCluster skinCluster_{};

	// 構造化バッファのリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> structuredBufferResource_{};
	// SRVのインデックス
	uint32_t srvIndex_{};
};