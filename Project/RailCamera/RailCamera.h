//#pragma once
////#include "../../object/Model/Model.h"
//#include "../../math/WorldTransform/WorldTransform.h"
//#include "../../object/Camera/Camera.h"
//
//class RailCamera {
//public:
//	/// <summary>
//	/// 初期化
//	/// </summary>
//	void Initialize(Vector3 translation, Vector3 rotate);
//
//	/// <summary>
//	/// 更新
//	/// </summary>
//	void Update();
//
//	/// <summary>
//	/// ビュープロジェクションを取得
//	/// </summary>
//	/// <returns>ビュープロジェクション</returns>
//	const Camera* GetCamera() { return camera_; }
//
//	/// <summary>
//	/// ワールド座標を取得
//	/// </summary>
//	WorldTransform& GetWorldTransform() { return worldTransform_; }
//
//	void SetCamera(Camera* camera) { this->camera_ = camera; }
//
//private:
//	// ワールド変換データ
//	WorldTransform worldTransform_;
//
//	Camera* camera_{};
//
//	// ビュープロジェクション
//	//ViewProjection viewProjection_;
//};

#pragma once

#include "../../math/MyMath.h"
#include "../../object/Camera/Camera.h"

/* RailCameraクラス */
class RailCamera {

public:

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize(Vector3 rotate, Vector3 translate);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// ワールド変換データを取得
	/// </summary>
	WorldTransform& GetWorldTransform() { return worldTransform_; };

	/// <summary>
	/// ビュープロジェクションの取得関数
	/// </summary>
	Camera* GetCamera() { return camera_.get(); }

private:

	/// <summary>
	/// フレームの更新処理
	/// </summary>
	void UpdateFrame(float& frame, uint32_t& index);

	Vector3 CatmullRomInt(uint32_t index, float t);

private:

	// ワールドトランスフォーム
	WorldTransform worldTransform_{};

	// ビュープロジェクション
	//Camera* camera_{};
	std::unique_ptr<Camera> camera_{};

	std::vector<Vector3> controlPoints_{};

	float t_{};

	float nowFrame_{};
	const float kEndFrame_ = 360.0f;

	// 視点
	float eye_T_;
	Vector3 eye_{};
	uint32_t eyeIndex_{};

	// 注視点
	float target_T_{};
	Vector3 target_{};
	uint32_t targetIndex_{};

	bool supurain_{};
};

