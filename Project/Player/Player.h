#pragma once
#include "../../Input/Input.h"
#include "../../math/MT/MT.h"
#include "../../object/Model/Model.h"
#include "../Bullet/PlayerBullet/PlayerBullet.h"
#include "../../math/WorldTransform/WorldTransform.h"
#include "../../object/Sprite/Sprite.h"
#include <list>

/// <summary>
/// 自キャラ
/// </summary>
class Player {
public:
	Player();
	~Player();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="textureHandle">テクスチャハンドル</param>
	void Initialize(Model* model, uint32_t textureHandle, Vector3 playerPosition);

	/// <summary>
	/// 更新
	/// </summary>
	void Update(ViewProjection viewProjection);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection viewProjection);

	/// <summary>
	/// 更新
	/// </summary>
	void Attack();

	/// <summary>
	/// 旋回
	/// </summary>
	void Rotate();

	/// <summary>
	/// ワールド座標を取得
	/// </summary>
	Vector3 GetWorldPosition();

	/// <summary>
	/// レティクルワールド座標を取得
	/// </summary>
	Vector3 GetReticleWorldPosition();

	// 衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

	/*WorldTransform GetWorldTransform() { return worldTransform_; }
	void SetWorldTransform_(WorldTransform worldTransform);*/

	// 弾リストを取得
	const std::list<PlayerBullet*>& GetBullets() const { return bullets_; }

	/// <summary>
	/// 親となるワールドトランスフォームをセット
	/// </summary>
	/// <param name="model">親となるワールドトランスフォーム</param>
	void SetParent(const WorldTransform* parent);

	/// <summary>
	/// UI描画
	/// </summary>
	void DrawUI();

	/// <summary>
	/// マウス座標を取得
	/// </summary>
	Vector2 GetCursorPosition();

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	// キーボード入力
	Input* input_ = nullptr;

	// 弾
	std::list<PlayerBullet*> bullets_;

	Vector3 offset = { 0, 0, 1.0f };

	uint32_t reticleTextureHandle_ = 0u;

	// 3Dレティクル用のトランスフォーム
	WorldTransform worldTransform3DReticle_;

	// 2Dレティクル用のスプライト
	Sprite* sprite2DReticle_ = nullptr;

	WorldTransform worldSprite2DReticle_;

	Vector4 reticleColor_{};
};
