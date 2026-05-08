#pragma once
#include "Object3D.h"

struct AABB
{
	VECTOR3 minPos;
	VECTOR3 maxPos;

	bool intersects(const AABB& other) const {
		if (minPos.x <= other.maxPos.x && maxPos.x >= other.minPos.x)
		{
			if (minPos.y <= other.maxPos.y && maxPos.y >= other.minPos.y)
			{
				if (minPos.z <= other.maxPos.z && maxPos.z >= other.minPos.z)
				{
					return true;
				}
			}
		}
		return false;
	}
};

struct CollisionInfo
{
	bool collided;
	VECTOR3 normal; // 衝突面の法線
	float push;
};

class Collider
{
public:
	Collider(VECTOR3 min, VECTOR3 max);
	~Collider();

	bool CheckBulletLineHitObject(VECTOR3 pos1, VECTOR3 pos2, VECTOR3* hit); // true → 銃弾に当たるオブジェクトがある
	bool CheckPushObject(VECTOR3* pushBack); // pushに押し返す値が代入される
	bool CheckOnGround(VECTOR3* pushBack); // pushに押し返す値が代入される

	void SetCollider(VECTOR3 min, VECTOR3 max) { aabb_ = { min, max }; }

private:
	AABB aabb_;
};
