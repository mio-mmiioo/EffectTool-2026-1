#pragma once
#include "../MyLibrary/Object3D.h"

namespace Collision
{
	void AddObject(Object3D* obj);
	bool CheckHitObject(VECTOR3 pos1, VECTOR3 pos2, VECTOR3* hit);
	bool CheckDistanceVertexAndVertex(VECTOR3 pos1, VECTOR3 pos2, float distance);
	void AttackedObject(int attackPower);
	void SetOnGround(Object3D* obj, float* velocity, const float gravity); // ‘«Œ³‚Ì“–‚½‚è”»’èŒãAˆÊ’u‚ğC³‚·‚é
	void CheckPush(Object3D* obj, VECTOR3 pos1, VECTOR3 pos2, float minDistance);
}
