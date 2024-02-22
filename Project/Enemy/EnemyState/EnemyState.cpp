#include "EnemyState.h"

void EnemyStateApproach::Initialize(Enemy* enemy) {
	enemy_ = enemy;
}

void EnemyStateApproach::Update(Enemy* pEnemy) {
	enemy_ = pEnemy;

	Vector3 move = { 0.0f, 0, -0.1f };

	enemy_->ChangePosition(move);
	// 既定の位置に到達したら離脱
	if (enemy_->GetWorldPosition().z <= -10.0f) {
		enemy_->ChangeState(new EnemyStateLeave());
	}
}

void EnemyStateLeave::Initialize(Enemy* enemy) {
	enemy_ = enemy;
}

void EnemyStateLeave::Update(Enemy* pEnemy) {
	enemy_ = pEnemy;

	Vector3 move = { 0.1f, 0.1f, 0 };
	// 移動

	enemy_->ChangePosition(move);
}
