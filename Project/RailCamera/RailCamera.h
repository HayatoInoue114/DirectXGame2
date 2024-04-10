#pragma once
//#include "../../object/Model/Model.h"
#include "../../math/WorldTransform/WorldTransform.h"
#include "../../object/Camera/Camera.h"

class RailCamera {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Vector3 translation, Vector3 rotate);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// ビュープロジェクションを取得
	/// </summary>
	/// <returns>ビュープロジェクション</returns>
	const Camera* GetCamera() { return camera_; }

	/// <summary>
	/// ワールド座標を取得
	/// </summary>
	WorldTransform& GetWorldTransform() { return worldTransform_; }

private:
	// ワールド変換データ
	WorldTransform worldTransform_;

	Camera* camera_{};

	// ビュープロジェクション
	//ViewProjection viewProjection_;
};
