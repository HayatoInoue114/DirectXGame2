#include "Enemy.h"
#include "../Player/Player.h"
#include "../../scene/GameScene/GameScene.h"

void (Enemy::* Enemy::situation[])() = { &Enemy::Approach, &Enemy::Leave };

Enemy::Enemy() {
	state_ = new EnemyStateApproach();
	/*player_ = new Player();*/
	deltaVector_ = {};
	velocity_ = {};
}

Enemy::~Enemy() {
	/*delete state_;
	delete player_;*/
}

void Enemy::Initialize(Model* model, Vector3 position) {
	// NULLポインタチェック
	assert(model);

	model_ = model;
	// テクスチャ読み込み
	textureHandle_ = PLAYER;

	worldTransform_.Initialize();
	// 引数で受け取った初期座標をセット
	worldTransform_.translation_ = { position };

	state_->Initialize(this);
}

void Enemy::Approach() {}

void Enemy::Leave() {
	// move = {kCharacterSpeed, kCharacterSpeed, 0};
	////// 移動
	// worldTransform_.translation_ = Add(worldTransform_.translation_, move);
}

void Enemy::ChangeState(BaseEnemyState* newState) {
	/*delete state_;*/
	state_ = newState;
}

void Enemy::Update() {
	state_->Update(this);

	worldTransform_.UpdateMatrix();
	worldTransform_.TransferMatrix();
}

void Enemy::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

void Enemy::ChangePosition(Vector3 vector) {
	worldTransform_.translation_ = Add(worldTransform_.translation_, vector);
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

void Enemy::OnCollision() {}
