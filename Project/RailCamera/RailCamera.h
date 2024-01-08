#pragma once
#include "../../object/Model/Model.h"
#include "../../math/WorldTransform/WorldTransform.h"


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
	const ViewProjection& GetViewProjection() { return viewProjection_; }

	/// <summary>
	/// ワールド座標を取得
	/// </summary>
	WorldTransform& GetWorldTransform() { return worldTransform_; }

private:
	// ワールド変換データ
	WorldTransform worldTransform_;

	// ビュープロジェクション
	ViewProjection viewProjection_;
};
