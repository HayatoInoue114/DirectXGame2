#pragma once

#pragma once
#include "../Enemy.h"
#include "../../TimedCall/TimedCall.h"

class Enemy;

class BaseEnemyState {
public:
	virtual void Initialize(Enemy* enemy) = 0;
	virtual void Update(Enemy* pEnemy) = 0;
};

// 近づく
class EnemyStateApproach : public BaseEnemyState {
	void Initialize(Enemy* enemy) override;
	void Update(Enemy* pEnemy) override;


private:
	Enemy* enemy_ = {};

};

// 逃げてく
class EnemyStateLeave : public BaseEnemyState {
	void Initialize(Enemy* enemy) override;
	void Update(Enemy* pEnemy) override;

private:
	Enemy* enemy_ = {};
};