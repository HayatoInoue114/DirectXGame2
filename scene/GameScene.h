#pragma once
#include "../ViewProjection/ViewProjection.h"
#include "../math/WorldTransform/WorldTransform.h"

#include "../Project/Player/Player.h"
#include "../Project/Enemy/Enemy.h"
#include "../Project/TimedCall/TimedCall.h"
#include "../Project/RailCamera/RailCamera.h"
#include "../Project/Skydome/Skydome.h"
#include "IScene.h"

class Enemy;

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene : public IScene{

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 衝突判定を応答
	/// </summary>
	void CheckAllCollisions();

	/// <summary>
	/// 敵弾を追加する
	/// </summary>
	/// <param name="enemyBullet">敵弾</param>
	void AddEnemyBullet(EnemyBullet* enemyBullet);

	/// <summary>
	/// 敵発生データの読み込み
	/// </summary>
	void LoadEnemyPopData();

	/// <summary>
	/// 敵発生コマンドの更新
	/// </summary>
	void UpdateEnemyPopCommands();

	// 敵を発生させる
	void EnemySpawn(Vector3 position);

	/// <summary>
	/// 敵弾発射
	/// </summary>
	void EnemyFire();

	void Draw2D();

	void Draw3D();

	/// <summary>
	/// 弾を発射し、タイマーをリセットするコールバック関数
	/// </summary>
	void FireAndResetCallback();

	void Finalize();

	bool GetIsClear() { return isClear_; }
private: // メンバ変数
	//DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	//Audio* audio_ = nullptr;

	WorldTransform worldTransform_;

	Sprite* blackSprite_;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
	/// //テクスチャハンドル
	uint32_t textureHandle = 0;

	// 3Dモデルデータ
	Model* model = nullptr;
	std::unique_ptr<Model> playerModel_;
	std::unique_ptr<Model> enemyModel_;

	// ビュープロジェクション
	ViewProjection viewProjection;

	// 自キャラ
	std::unique_ptr<Player> player_;

	// デバッグカメラ有効
	bool isDebugCameraActive_ = false;

	// デバッグカメラ
	//DebugCamera* debugCamera_ = nullptr;

	// 敵
	std::list<Enemy*> enemies_;
	std::list<Model*> enemiesModel_;
	//std::unique_ptr<Model> enemyModel_;

	// 天球
	std::unique_ptr<Skydome> skydome_;
	//Skydome* skydome_ = nullptr;

	// 3Dモデル
	std::unique_ptr<Model> modelSkydome_ = nullptr;

	// レールカメラ
	RailCamera* railCamera_ = nullptr;

	// 弾
	std::list<EnemyBullet*> enemyBullets_;

	// 敵発生コマンド
	std::stringstream enemyPopCommands;

	bool isWaiting_ = false;
	int waitTimer_ = 0;

	// 弾を発射する間隔
	static const uint32_t kFireInterval = 100;
	// 次元発動のリスト
	std::list<TimedCall*> timedCalls_;

	int sceneNum_{};

	int dedCount_{};

	int flameCount_{};

	bool isClear_{};
};

