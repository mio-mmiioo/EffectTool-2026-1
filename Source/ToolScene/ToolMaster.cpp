#include "ToolMaster.h"
#include "../../ImGui/imgui.h"
#include "../MyLibrary/Light.h"
#include "Stage/Stage.h"
#include "Collision.h"
#include "User.h"

namespace ToolMaster
{
	void ImGuiInput(); // ImGuiで、開発時のみ使用する処理

	// 開発時のみ使用
	const float CHECK_FRONT_LENGTH = 100.0f;
	const float CHECK_BACK_LENGTH = 100.0f;

	User* user = nullptr;
	Stage* stage = nullptr;

	VECTOR3 playerPosition;
	bool isCreateEnemy; // 敵を作成するか true → 作成する
	bool isDrawWayInfo; 
}

void ToolMaster::Init()
{
	new Stage(12); // 建物だけのステージ
	Light::Init();
}

void ToolMaster::Update()
{
	Light::Update();
}

void ToolMaster::Draw()
{

}

void ToolMaster::Release()
{

}

bool ToolMaster::IsBulletHit(VECTOR3 startPosition, VECTOR3 endPosition)
{
	// 敵、破壊可能オブジェクトにあたるならtrueを返す
	VECTOR3 hit;
	if (Collision::CheckHitObject(startPosition, endPosition, &hit) == true)
	{
		return true;
	}
	return false;
}

void ToolMaster::CheckSetPosition(Object3D* obj, float* velocityY, float distanceR, float gravity)
{
	Transform t = obj->GetTransform();
	VECTOR3 front = t.position_ + VECTOR3(0, 0, 1) * CHECK_FRONT_LENGTH * MGetRotY(t.rotation_.y);
	VECTOR3 back = t.position_ + VECTOR3(0, 0, 1) * -CHECK_BACK_LENGTH * MGetRotY(t.rotation_.y);
	Collision::CheckPush(obj, t.position_, front, distanceR); // ステージへのめり込みを確認する(前方)
	Collision::CheckPush(obj, t.position_, back, distanceR);  // ステージへのめり込みを確認する(後方)
	Collision::SetOnGround(obj, velocityY, gravity); // ステージの位置を確認し、空中に浮いていないか確認する 浮いていたら重力をかける
}

void ToolMaster::ImGuiInput()
{

}
