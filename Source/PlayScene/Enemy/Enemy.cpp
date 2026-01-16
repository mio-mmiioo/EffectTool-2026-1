#include "Enemy.h"
#include "../../../ImGui/imgui.h"
#include "../../MyLibrary/Observer.h"
#include "../../MyLibrary/Object3D.h"
#include "../Stage/WayInfo.h"
#include "../GameMaster.h"
#include "../Collision.h"
#include <assert.h>
#include <map>
// 各敵のヘッダーファイル
#include "Stone.h"
#include "Touhu.h"

namespace Enemy
{
	struct E_DATA {
		float distanceCurrentAndGo;
		float distanceThisAndPlayer;
	};

	std::string SetCurrentEnemySort(int sortNumber);

	std::map<std::string, E_DATA> data;
}

void Enemy::Init()
{
	data["STONE"] = { 20.0f, 200.0f };
	data["TOUHU"] = { 20.0f, 200.0f };
}

void Enemy::DevelopmentInput(Transform& t)
{
	//ImGui::Begin("Enemy");
	ImGui::Text("position");
	ImGui::InputFloat("position:x", &t.position_.x);
	ImGui::InputFloat("position:y", &t.position_.y);
	ImGui::InputFloat("position:z", &t.position_.z);

	ImGui::Text("rotation");
	ImGui::InputFloat("rotation:x", &t.rotation_.x);
	ImGui::InputFloat("rotation:y", &t.rotation_.y);
	ImGui::InputFloat("rotation:z", &t.rotation_.z);

	ImGui::Text("scale");
	ImGui::InputFloat("scale:x", &t.scale_.x);
	ImGui::InputFloat("scale:y", &t.scale_.y);
	ImGui::InputFloat("scale:z", &t.scale_.z);

	//ImGui::End();
}

VECTOR3 Enemy::GetMoveToPlayerPosition(VECTOR3 position)
{
	VECTOR3 playerPosition = GameMaster::GetPlayerPosition();
	VECTOR3 ret = WayInfo::GetShortestWayPosition(position, playerPosition);
	return ret;
}

void Enemy::CreateEnemy(int enemyNumber, const std::string& fileName, const Transform& t, int hp, int score)
{
	switch (enemyNumber)
	{
	case E_SORT::STONE:
		new Stone(fileName, t, hp, score);
		break;
	case E_SORT::TOUHU:
		new Touhu(fileName, t, hp, score);
		break;
	}
}

// プレイヤーとの距離が開きすぎると、状態が遷移する
void Enemy::StayUpdate(int sortNumber, VECTOR3 ePosition, E_STATE* state, E_STATE nextState)
{
	std::string str = SetCurrentEnemySort(sortNumber);
	VECTOR3 pPosition = GameMaster::GetPlayerPosition();
	float distance = data[str].distanceThisAndPlayer;
	if (Collision::CheckDistanceVertexAndVertex(ePosition, pPosition, distance) == false)
	{
		*state = nextState;
	}
}

// プレイヤーまでを最短経路で移動する
void Enemy::WalkUpdate(int sortNumber, bool* isArrive, VECTOR3* goPosition, VECTOR3* ePosition, E_STATE* state, E_STATE nextState)
{
	std::string str = SetCurrentEnemySort(sortNumber);

	if (*isArrive == true)
	{
		*goPosition = Enemy::GetMoveToPlayerPosition(*ePosition);
		*isArrive = false;
	}
	else
	{
		if (Collision::CheckDistanceVertexAndVertex(*ePosition, *goPosition, data[str].distanceCurrentAndGo) == true)
		{
			*isArrive = true;
		}
	}

	if (Collision::CheckDistanceVertexAndVertex(*ePosition, GameMaster::GetPlayerPosition(), data[str].distanceThisAndPlayer))
	{
		*state = nextState;
	}
}

std::string Enemy::SetCurrentEnemySort(int sortNumber)
{
	std::string ret;
	switch (sortNumber)
	{
	case E_SORT::STONE:
		ret = "STONE";
		break;
	case E_SORT::TOUHU:
		ret = "TOUHU";
		break;
	}
	return ret;
}
