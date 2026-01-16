#include "Stone.h"
#include <assert.h>
#include "../../../ImGui/imgui.h"
#include "../../MyLibrary/Observer.h"
#include "../GameMaster.h"
#include "../Collision.h"
#include "Enemy.h"

namespace STONE
{
	const float G = 0.05f;

	const float MOVE_SPEED = 3.0f;
	const float ROTATE_SPEED = 3.0f;
	const float DISTANCE_R = 50.0f;

	VECTOR3 headshotPos = { 0.0f, 170.0f, 0.0f };
	float headshotR = 25.0f;
	int headshotBonus = 2;

	// 攻撃関連
	const float ATTACK_TIME = 6.0f;
	const int ATTACK_POWER = 2;

	// 開発時のみ使用
	const float DIRECTION_LENGTH = 100.0f;
	const float POS_LIST_R = 50.0f;
	const int POS_LIST_DIV_NUM = 50;
}

Stone::Stone(const std::string& fileName, const Transform& t, int hp, int score)
{
	const std::string folder = "data/model/";
	hModel_ = MV1LoadModel((folder + fileName + ".mv1").c_str());
	assert(hModel_ > 0);
	hitModel_ = MV1LoadModel((folder + fileName + "_c.mv1").c_str());
	assert(hitModel_ > 0);

	transform_ = t;
	hp_ = hp;
	score_ = score;

	time_ = 0.0f;
	gravity_ = STONE::G;
	distanceR_ = STONE::DISTANCE_R;


	transform_.MakeLocalMatrix();
	MV1SetMatrix(hitModel_, transform_.GetLocalMatrix());
	MV1SetupCollInfo(hitModel_);

	rotateSpeed_ = STONE::ROTATE_SPEED;
	moveSpeed_ = STONE::MOVE_SPEED;

	Collision::AddObject(this);

	objectNumber_ = OBJECT_SORT::OBJ_CHARA;
	state_ = Enemy::E_STATE::STAY;
}

Stone::~Stone()
{
	if (hModel_ > 0)
	{
		hModel_ = -1;
	}
	if (hitModel_ > 0)
	{
		hitModel_ = -1;
	}
}

void Stone::Update()
{
	// 体力が0の場合の処理
	if (hp_ <= 0)
	{
		DestroyMe();
		Observer::AddScore(score_);
		return;
	}

	switch (state_)
	{
	case Enemy::E_STATE::STAY:
		UpdateStay();
		break;
	}

	DevelopmentInput();
	
	GameMaster::CheckSetPosition(this, &velocityY_, distanceR_, gravity_);

	MV1SetMatrix(hitModel_, transform_.GetLocalMatrix());
	MV1RefreshCollInfo(hitModel_);
}

void Stone::Draw()
{
	Object3D::Draw();
}

void Stone::DevelopmentInput()
{
	int s = state_;
	ImGui::Begin("Stone");

	ImGui::RadioButton("Stay", &s, Enemy::E_STATE::STAY);

	Enemy::DevelopmentInput(transform_);

	switch (s)
	{
	case Enemy::E_STATE::STAY:
		state_ = Enemy::E_STATE::STAY;
		break;
	}

	ImGui::End();

	transform_.MakeLocalMatrix();
}

void Stone::UpdateStay()
{
}
