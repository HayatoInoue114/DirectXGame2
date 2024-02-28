#include "Enemy.h"
#include "../Player/Player.h"
#include "../../scene/GameScene.h"
#include "EnemyState/EnemyState.h"

void (Enemy::* Enemy::situation[])() = { &Enemy::Approach, &Enemy::Leave };

Enemy::Enemy() {
	state_ = new EnemyStateApproach();
	/*player_ = new Player();*/
	deltaVector_ = {};
	velocity_ = {};
	isDead = true;
}

Enemy::~Enemy() {
	/*delete state_;
	delete player_;*/
}

void Enemy::Initialize(Particle* model, Vector3 position) {
	// NULLポインタチェック
	assert(model);

	model_ = model;
	// テクスチャ読み込み
	textureHandle_ = 0;

	isDead = true;

	worldTransform_.Initialize();
	// 引数で受け取った初期座標をセット
	worldTransform_.translate = { position };

	state_ = new EnemyStateApproach();

	state_->Initialize(this);
}

void Enemy::Approach() {}

void Enemy::Leave() {
	// move = {kCharacterSpeed, kCharacterSpeed, 0};
	////// 移動
	// worldTransform_.translate = Add(worldTransform_.translate, move);
}

void Enemy::ChangeState(BaseEnemyState* newState) {
	delete state_;
	state_ = newState;
}

void Enemy::Update() {
	state_->Update(this);

	

	worldTransform_.UpdateMatrix();
	worldTransform_.TransferMatrix();
}

void Enemy::Draw(ViewProjection& viewProjection) {
	//model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

void Enemy::ChangePosition(Vector3 vector) {
	worldTransform_.translate = Add(worldTransform_.translate, vector);
}

void Enemy::Fire() { gameScene_->EnemyFire(); }

Vector3 Enemy::GetWorldPosition() {
	// ワールド座標を入れる変数
	Vector3 worldPos = {};
	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldPos = {
		worldTransform_.matWorld_.m[3][0], worldTransform_.matWorld_.m[3][1],
		worldTransform_.matWorld_.m[3][2] };

	return worldPos;
}

void Enemy::OnCollision() {
	isDead = true;
}
