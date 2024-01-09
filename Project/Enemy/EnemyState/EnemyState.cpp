#include "EnemyState.h"

void EnemyStateApproach::Initialize(Enemy* enemy) {
	enemy_ = enemy;
}

void EnemyStateApproach::Update(Enemy* pEnemy) {
	enemy_ = pEnemy;
	Vector3 position = enemy_->GetWorldPosition();

	Vector3 move = { 0.00f, 0, -0.04f };

	enemy_->ChangePosition(move);
	// 既定の位置に到達したら離脱
	if (position.z < -10.0f) {
		enemy_->ChangeState(new EnemyStateLeave());
	}
}

void EnemyStateLeave::Initialize(Enemy* enemy) {
	enemy_ = enemy;
}

void EnemyStateLeave::Update(Enemy* pEnemy) {
	enemy_ = pEnemy;
	Vector3 position = enemy_->GetWorldPosition();

	Vector3 move = { 0.1f, 0.1f, 0 };
	// 移動

	enemy_->ChangePosition(move);
}
