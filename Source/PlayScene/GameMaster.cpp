#include "GameMaster.h"
#include "../../ImGui/imgui.h"
#include "../MyLibrary/Light.h"
//#include "../MyLibrary/Shadow.h"

#include "Stage/Stage.h"
#include "Stage/WayInfo.h"
#include "Enemy/Enemy.h"
#include "Player.h"
#include "HP.h"
#include "Collision.h"

namespace GameMaster
{
	void DevelopmentInput(); // ImGuiで、開発時のみ使用する処理
	void PlayerUpdate(); // GameMaster内で処理したいPlayerのUpdateの内容

	// 開発時のみ使用
	const float CHECK_FRONT_LENGTH = 100.0f;
	const float CHECK_BACK_LENGTH = 100.0f;

	Player* player = nullptr;
	Stage* stage = nullptr;

	VECTOR3 playerPosition;
	bool isCreateEnemy; // 敵を作成するか true → 作成する
	bool isDrawWayInfo; // WayInfoを描画するか true → 作成する
}

void GameMaster::Init()
{
	WayInfo::Init();
	new Stage(12); // 建物だけのステージ
	Enemy::Init();
	Light::Init();
	//Shadow::Init(); // Stageの後に処理する
}

void GameMaster::Update()
{
	Light::Update();
	player = FindGameObject<Player>();
	stage = FindGameObject<Stage>();

	if (player->HP_->GetHP() < 0)
	{
		SceneManager::ChangeScene("RESULT");
	}

	// 開発時関連
	{
		DevelopmentInput();

		if (isCreateEnemy == true)
		{
			new Stage(13);
			isCreateEnemy = false;
		}
	}

	PlayerUpdate();

	if (player->GetIsAttack() == true)
	{
		Collision::AttackedObject(-player->GetAttackPower());
	}
}

void GameMaster::Draw()
{
	if (isDrawWayInfo == true) WayInfo::WayDraw();
}

void GameMaster::Release()
{
}

bool GameMaster::IsBulletHit(VECTOR3 startPosition, VECTOR3 endPosition)
{
	// 敵、破壊可能オブジェクトにあたるならtrueを返す
	VECTOR3 hit;
	if (Collision::CheckHitObject(startPosition, endPosition, &hit) == true)
	{
		return true;
	}
	return false;
}

// 現在地が地面や壁にめり込んでいる場合、押し返す
void GameMaster::CheckSetPosition(Object3D* obj, float* velocityY, float distanceR, float gravity)
{
	Transform t = obj->GetTransform();
	VECTOR3 front = t.position_ + VECTOR3(0, 0, 1) * CHECK_FRONT_LENGTH * MGetRotY(t.rotation_.y);
	VECTOR3 back = t.position_ + VECTOR3(0, 0, 1) * -CHECK_BACK_LENGTH * MGetRotY(t.rotation_.y);
	Collision::CheckPush(obj, t.position_, front, distanceR); // ステージへのめり込みを確認する(前方)
	Collision::CheckPush(obj, t.position_, back, distanceR);  // ステージへのめり込みを確認する(後方)
	Collision::SetOnGround(obj, velocityY, gravity); // ステージの位置を確認し、空中に浮いていないか確認する 浮いていたら重力をかける
}

VECTOR3 GameMaster::GetPlayerPosition()
{
	return playerPosition;
}

void GameMaster::AttackPlayer(int attackPower)
{
	player->AddHp(-attackPower);
}

void GameMaster::DevelopmentInput()
{
	ImGui::Begin("GameMaster");
	ImGui::Checkbox("Draw WayInfo", &isDrawWayInfo);
	if (ImGui::Button("CreateEnemy"))
	{
		isCreateEnemy = true;
	}
	ImGui::End();
}

void GameMaster::PlayerUpdate()
{
	VECTOR3 p = player->GetTransform().position_;
	if (WayInfo::IsVertexPosition(p))
	{
		playerPosition = p;
	}
}
