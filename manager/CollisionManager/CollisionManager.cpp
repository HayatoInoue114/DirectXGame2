//#include "CollisionManager.h"
//
//void CollisionManager::Initialize() {
//
//}
//
//void CollisionManager::Update() {
//	colliders_.clear();
//	Colliders();
//	CheckAllCollision();
//}
//void CollisionManager::Colliders() {
//	const std::list<Bullet*>& playerBullets_ = player_->GetBullets();
//	const std::list<EnemyBullet*>& enemyBullets_ = enemy_->GetBullets();
//
//	// std::list<Collider*> colliders_;
//	colliders_.push_back(player_);
//	colliders_.push_back(enemy_);
//
//	for (Bullet* playerBullet : playerBullets_) {
//		colliders_.push_back(playerBullet);
//	}
//	for (EnemyBullet* enemyBullet : enemyBullets_) {
//		colliders_.push_back(enemyBullet);
//	}
//}
//void CollisionManager::CheckAllCollision() {
//	// リストのペアを総当たり
//	std::list<Collider*>::iterator itrA = colliders_.begin();
//	for (; itrA != colliders_.end(); ++itrA) {
//		Collider* colliderA = *itrA;
//
//		// イテレーターBはイテレーターAの次の要素から回す
//		std::list<Collider*>::iterator itrB = itrA;
//		itrB++;
//		for (; itrB != colliders_.end(); ++itrB) {
//			Collider* colliderB = *itrB;
//
//			// ペアの当たり判定
//			CheckCollisionPair(colliderA, colliderB);
//		}
//	}
//}
//void CollisionManager::CheckCollisionPair(Collider* colliderA, Collider* colliderB) {
//	if ((colliderA->GetCollisionAttribute() & colliderB->GetCollisionMask()) == 0 ||
//		(colliderA->GetCollisionMask() & colliderB->GetCollisionAttribute()) == 0) {
//		return;
//	}
//	Vector3 posA = colliderA->GetWorldPosition();
//	Vector3 posB = colliderB->GetWorldPosition();
//	float length = Length(Subtract(posA, posB));
//	// 交差判定
//	if (length <= colliderA->GetRadius() + colliderB->GetRadius()) {
//		colliderA->OnCollision();
//		colliderB->OnCollision();
//	}
//}