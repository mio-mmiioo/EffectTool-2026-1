#define NOMINMAX
#include "Collider.h"
#include <algorithm>

namespace Collision
{
	const VECTOR3 CHECK_ONGROUND_LENGTH = { 0.0f, 5000.0f, 0.0f };
	const float HIGHEST_Y = 5000.0f; // 一番高いオブジェクトのY座標
	std::list<Object3D*> objectList; // すべての銃弾が当たるオブジェクト
	std::list<AABB*> hitObjectList; // 当たり判定用の簡易的な当たり判定
	Object3D* hitObject = nullptr;
}

Collider::Collider(VECTOR3 min, VECTOR3 max)
{
	aabb_.minPos = min;
	aabb_.maxPos = max;

	Collision::hitObjectList.push_back(&aabb_);
}

Collider::~Collider()
{
}

bool Collider::CheckBulletLineHitObject(VECTOR3 pos1, VECTOR3 pos2, VECTOR3* hit)
{
	return false;
}

bool Collider::CheckPushObject(VECTOR3* pushBack)
{
	for (auto itr = Collision::hitObjectList.begin(); itr != Collision::hitObjectList.end(); itr++)
	{
		if (aabb_.intersects(AABB{ (*itr)->minPos, (*itr)->maxPos }) == true)
		{

		}

	}
	return false;
}

bool Collider::CheckOnGround(VECTOR3* pushBack)
{
	//CollisionInfo info;
	bool ret = false;
	AABB hit;
	// オブジェクトと床の当たり判定をする
	for (auto itr = Collision::hitObjectList.begin(); itr != Collision::hitObjectList.end(); itr++)
	{
		if (aabb_.minPos.y <= (*itr)->maxPos.y && aabb_.maxPos.y >= (*itr)->minPos.y)
		{
			// 床とめり込んでいる
			ret = true;
			hit = { (*itr)->minPos, (*itr)->maxPos };
			break;
		}
	}

	// 衝突したオブジェクトがない場合、処理終了
	if (ret == false)
	{
		return false;
	}

	// 衝突箇所から、めり込み量を計算する
	float distance = std::min(aabb_.maxPos.y - hit.minPos.y, aabb_.maxPos.y - hit.minPos.y);
	
	// 押し戻す方向を計算する
	// 押し戻す量を代入する

	return true;
}

//Object3D* Collider::GetBulletHitObject()
//{
//	return nullptr;
//}
