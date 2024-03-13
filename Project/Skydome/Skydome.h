#pragma once
//#include "../../object/Model/Model.h"
#include "../../math/WorldTransform/WorldTransform.h"


class Particle;
class Skydome {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Particle* model);
	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private:
	// ワールド変換データ
	WorldTransform worldTransform_;

	// モデル
	Particle* model_ = nullptr;
};
