#pragma once
//#include "../../object/Model/Model.h"
#include "../../math/WorldTransform/WorldTransform.h"
#include "../../object/Camera/Camera.h"
#include "../../math/MyMath.h"

class Model;
class Skydome {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model);
	void Initialize(Model* model, Camera* camera);
	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	void SetCamera(Camera* camera) { this->camera_ = camera; }

private:
	// ワールド変換データ
	WorldTransform worldTransform_;

	Camera* camera_;

	// モデル
	Model* model_ = nullptr;
};
