#pragma once
#include "../../object/Primitive/Primitive/Primitive.h"
#include "../../object/Camera/Camera.h"
#include "../../object/Model/Model.h"
#include "../../object/Particle/Particle.h"
#include "../../Project/Skydome/Skydome.h"
#include "../../Project/RailCamera/RailCamera.h"
#include "../../Project/Player/Player.h"
#include "../../Project/Enemy/Enemy.h"
#include "../../object/Object3d/Object3d.h"
#include <memory>

class GraphicsRenderer;

class PrimitiveManager
{
public:
	void Initialize();

	void Update();

	void Draw();

	void Finalize();

private:
	void Draw2D();

	void Draw3D();

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
	void FireAndResetCallback();

private:
	std::unique_ptr<Primitive> primitive_;

	std::unique_ptr<Model> model_;
	std::unique_ptr<Object3d> playerObject_;

	std::unique_ptr<Primitive> primitive2_;

	//std::unique_ptr<PrimitiveCommon> primitiveCommon_;

	std::unique_ptr<Camera> camera_;

	std::unique_ptr<Skydome> dome_;

	std::unique_ptr<Model> domeModel_;
	std::unique_ptr<Object3d> domeObject_;

	std::unique_ptr<RailCamera> railCamera_;

	//std::unique_ptr<Particle> particle_{};

	Particle particle_;

	std::unique_ptr<Player> player_;
	std::list<Enemy*> enemies_;
	std::list<Model*> enemiesModel_;

	// 弾
	std::list<EnemyBullet*> enemyBullets_;

	// 敵発生コマンド
	std::stringstream enemyPopCommands{};

	bool isWaiting_ = false;
	int waitTimer_ = 0;

	// 弾を発射する間隔
	static const uint32_t kFireInterval = 100;
	// 次元発動のリスト
	std::list<TimedCall*> timedCalls_;
};

