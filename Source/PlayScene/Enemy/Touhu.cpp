#include "Touhu.h"
#include "../../../ImGui/imgui.h"
#include "../../MyLibrary/Observer.h"
#include "../../MyLibrary/Color.h"
#include "../GameMaster.h"
#include "../Collision.h"
#include "Enemy.h"
#include <assert.h>

namespace TOUHU
{
	const float G = 0.05f;

	const float MOVE_SPEED = 3.0f;
	const float ROTATE_SPEED = 3.0f;
	const float DISTANCE_R = 50.0f;

	VECTOR3 headshotPos = { 0.0f, 170.0f, 0.0f };
	float headshotR = 25.0f;
	int headshotBonus = 2;

	// 攻撃関連
	const float STAY_TIME = 6.0f;
	const int ATTACK_POWER = 2;
	
	// 開発時のみ使用
	const float DIRECTION_LENGTH = 100.0f;
	const float POS_LIST_R = 50.0f;
	const int POS_LIST_DIV_NUM = 50;
}

Touhu::Touhu(const std::string& fileName, const Transform& t, int hp, int score)
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
	gravity_ = TOUHU::G;
	distanceR_ = TOUHU::DISTANCE_R;


	transform_.MakeLocalMatrix();
	MV1SetMatrix(hitModel_, transform_.GetLocalMatrix());
	MV1SetupCollInfo(hitModel_);

	rotateSpeed_ = TOUHU::ROTATE_SPEED;
	moveSpeed_ = TOUHU::MOVE_SPEED;

	Collision::AddObject(this);

	goPosition_ = transform_.position_;
	objectNumber_ = OBJECT_SORT::OBJ_CHARA;
	state_ = Enemy::E_STATE::STAY;
	stayTimer_ = 0.0f;
	isArrive_ = true;
}

Touhu::~Touhu()
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

void Touhu::Update()
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
		Enemy::StayUpdate(Enemy::E_SORT::TOUHU, transform_.position_, &state_, Enemy::E_STATE::WALK);
		stayTimer_ += Time::DeltaTime();
		if (stayTimer_ > TOUHU::STAY_TIME)
		{
			stayTimer_ -= TOUHU::STAY_TIME;
			state_ = Enemy::E_STATE::ATTACK;
		}
		break;
	case Enemy::E_STATE::WALK:
		Enemy::WalkUpdate(Enemy::E_SORT::TOUHU, &isArrive_, &goPosition_, &transform_.position_, &state_, Enemy::E_STATE::STAY);
		if (isArrive_ == false)
		{
			SetMove(goPosition_);
		}
		break;
	case Enemy::E_STATE::ATTACK:
		GameMaster::AttackPlayer(TOUHU::ATTACK_POWER);
		state_ = Enemy::E_STATE::STAY;
		break;
	}

	DevelopmentInput();

	GameMaster::CheckSetPosition(this, &velocityY_, distanceR_, gravity_);


	// 位置情報の更新
	
	MV1SetMatrix(hitModel_, transform_.GetLocalMatrix());
	MV1RefreshCollInfo(hitModel_);
}

void Touhu::Draw()
{
	Object3D::Draw();

	// 向いている方向
	DrawLine3D(transform_.position_ + LOOK_HEIGHT,
		transform_.position_ + LOOK_HEIGHT + VECTOR3(0, 0, 1) * TOUHU::DIRECTION_LENGTH * MGetRotY(transform_.rotation_.y), Color::BLACK);
}

void Touhu::DevelopmentInput()
{
	int s = state_;
	ImGui::Begin("Touhu");

	ImGui::RadioButton("Stay", &s, Enemy::E_STATE::STAY);
	ImGui::RadioButton("Walk", &s, Enemy::E_STATE::WALK);
	ImGui::RadioButton("Attack", &s, Enemy::E_STATE::ATTACK);
	Enemy::DevelopmentInput(transform_);

	switch (s)
	{
	case Enemy::E_STATE::STAY:
		state_ = Enemy::E_STATE::STAY;
		break;
	case Enemy::E_STATE::WALK:
		state_ = Enemy::E_STATE::WALK;
		break;
	case Enemy::E_STATE::ATTACK:
		state_ = Enemy::E_STATE::ATTACK;
		break;
	}

	ImGui::End();

	transform_.MakeLocalMatrix();
}

