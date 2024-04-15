#pragma once
#include "../../../math/MyMath.h"
#include "../CollisionConfig/CollisionConfig.h"

class Collider {
public:
	virtual void OnCollision() = 0;

	virtual Vector3 GetWorldPosition() = 0;

	float GetRadius() { return radius_; }
	void SetRadius(float radius) { radius_ = radius; }

	// 属性
	uint32_t GetCollisionAttribute() { return collisionAttribute_; }
	void SetCollisionAttribute(uint32_t collisionAttribute) {
		collisionAttribute_ = collisionAttribute;
	}
	// マスク
	uint32_t GetCollisionMask() { return collisionMask_; }
	void SetCollisionMask(uint32_t collisionMask) { collisionMask_ = collisionMask; }

private:
	// 半径
	float radius_ = 2;

	uint32_t collisionAttribute_ = 0xFFFFFFFF;
	uint32_t collisionMask_ = 0xFFFFFFFF;
};