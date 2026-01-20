#include "Collision.h"
#include "../MyLibrary/Observer.h"

namespace Collision
{
	const VECTOR3 CHECK_ONGROUND_LENGTH = { 0.0f, 5000.0f, 0.0f };
	std::list<Object3D*> allObjectList; // すべての銃弾が当たるオブジェクト
	Object3D* checkObject;
}

void Collision::AddObject(Object3D* obj)
{
	if (obj != nullptr)
	{
		allObjectList.push_back(obj);
	}
}

bool Collision::CheckHitObject(VECTOR3 pos1, VECTOR3 pos2, VECTOR3* hit)
{
	bool found = false;
	VECTOR3 now;
	float nowVal = ((VECTOR3)(pos2 - pos1)).Size();

	for (Object3D* obj : allObjectList)
	{
		if (obj->GetObjectNumber() != OBJECT_SORT::OBJ_PLAYER)
		{
			VECTOR3 ret;
			if (obj->Object3D::CollideLine(pos1, pos2, &ret))
			{
				found = true;
				VECTOR3 v = pos1 - ret;
				float len = v.Size();
				if (len < nowVal)
				{
					nowVal = len;
					now = ret;
					checkObject = obj;
				}
			}
		}
	}
	if (hit != nullptr)
	{
		*hit = now;
		Observer::SetHitPosition(*hit);
	}

	// hpが0以下で存在する → 壊れないオブジェクト
	if (checkObject != nullptr)
	{
		if (checkObject->GetHp() <= 0)
		{
			found = false;
		}
	}
	return found;
}

bool Collision::CheckDistanceVertexAndVertex(VECTOR3 pos1, VECTOR3 pos2, float distance)
{
	if (VSize(pos1 - pos2) < distance)
	{
		return true;
	}
	return false;
}

void Collision::AttackedObject(int attackPower)
{
	if (checkObject != nullptr)
	{
		checkObject->AddHp(attackPower);
		Observer::SetAttackPower(attackPower);
	}
}

void Collision::AddVelocity(Object3D* obj, float* velocityY, const float gravity)
{
	VECTOR3 p = obj->GetTransform().position_;
	// 重力を加える
	p.y -= *velocityY;
	*velocityY += gravity;
	obj->SetPosition(p);
}

bool Collision::SetOnGround(Object3D* obj)
{
	bool ret = false;
	VECTOR3 hit;
	VECTOR3 p = obj->GetTransform().position_;

	// 重力を加える
	//p.y -= *velocityY;
	//*velocityY += gravity;
	//obj->SetPosition(p);

	VECTOR3 pos1 = p + CHECK_ONGROUND_LENGTH;
	VECTOR3 pos2 = p - CHECK_ONGROUND_LENGTH;

	for (Object3D* o : allObjectList)
	{
		if (obj != o)
		{
			if (o->CollideLine(pos1, pos2, &hit))
			{
				if (p.y < hit.y)
				{
					// めり込んでる
					VECTOR3 pos = p - VECTOR3(0.0f, p.y - hit.y, 0.0f);
					obj->SetPosition(pos);
					ret = true;
				}
			}
		}
	}
	return ret;
}

void Collision::CheckPush(Object3D* obj, VECTOR3 pos1, VECTOR3 pos2, float minDistance)
{
	VECTOR3 hit;
	VECTOR3 direction;
	for (Object3D* o : allObjectList)
	{
		if (o->CollideLine(pos1, pos2, &hit)) // 正面にオブジェクトがある
		{
			if (VSize(pos1 - hit) < minDistance) // めり込んでいる→めり込んでいる距離押し返す
			{
				direction = VNorm(hit - pos1); // 押し返す方向のベクトル
				pos1 -= direction * (minDistance - VSize(pos1 - hit)); // ( 押し返す方向 ) * ( 押し返したい距離 )
				obj->SetPosition(pos1);
			}
		}
	}
}

void Collision::SetSelectObject()
{
	checkObject->SetSelect(true);
}

void Collision::DeselectObject()
{
	checkObject->SetSelect(false);
}

