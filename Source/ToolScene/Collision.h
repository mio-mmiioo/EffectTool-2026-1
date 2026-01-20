#pragma once
#include "../MyLibrary/Object3D.h"

namespace Collision
{
	void AddObject(Object3D* obj);
	bool CheckHitObject(VECTOR3 pos1, VECTOR3 pos2, VECTOR3* hit);
	bool CheckDistanceVertexAndVertex(VECTOR3 pos1, VECTOR3 pos2, float distance);
	void AttackedObject(int attackPower);
	void AddVelocity(Object3D* obj, float* velocityY, const float gravity);
	bool SetOnGround(Object3D* obj); // 足元の当たり判定後、位置を修正する 地面に足がついた場合、trueが返る
	void CheckPush(Object3D* obj, VECTOR3 pos1, VECTOR3 pos2, float minDistance);
	void SetSelectObject();
	void DeselectObject();
}
