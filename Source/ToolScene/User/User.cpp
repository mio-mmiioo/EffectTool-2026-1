#include "User.h"
#include <assert.h>
#include "../../../ImGui/imgui.h"
#include "../../MyLibrary/Input.h"
#include "../ToolMaster.h"
#include "../Collision.h"
#include "Camera.h"
#include "Gun.h"

namespace USER
{
	const float G = 0.05;	// 重力
	float ROTATE_SPEED = 3.0f;	// 回転速度
	float MOVE_SPEED = 5.0f;	// 移動速度

	const VECTOR3 CAPSULE_POS1 = { 0.0f,  50.0f, 0.0f };
	const VECTOR3 CAPSULE_POS2 = { 0.0f, 150.0f, 0.0f };
	const float DISTANCE_R = 100.0f;

	// 当たり判定
	VECTOR3 AABB_MIN = { -50.0f, 0.0f, -50.0f };
	VECTOR3 AABB_MAX = { 50.0f, 200.0f, 50.0f };


	// 開発時のみ
	const float DIRECTION_LENGTH = 100.0f; // 向いている方向


}

User::User(const VECTOR3& position)
{
	objectNumber_ = OBJECT_SORT::OBJ_PLAYER;
	transform_.position_ = position;
	hp_ = 10000;
	hModel_ = MV1LoadModel("data/model/player.mv1");
	assert(hModel_ > 0);
	hitModel_ = MV1LoadModel("data/model/player_c.mv1");
	assert(hitModel_ > 0);

	time_ = 0.0f;
	gravity_ = USER::G;
	distanceR_ = USER::DISTANCE_R;

	transform_.MakeLocalMatrix();
	MV1SetupCollInfo(hModel_);

	// 照準(aiming)
	SetAimingImage(aiming_, "data/image/pointer1.png");
	SetAimingImage(hitAiming_, "data/image/pointer2.png");

	rotateSpeed_ = USER::ROTATE_SPEED;
	moveSpeed_ = USER::MOVE_SPEED;

	camera_ = new Camera();
	Gun::Init();

	collider_ = new Collider(USER::AABB_MIN, USER::AABB_MAX);

	SetDrawOrder(-100);
}

User::User()
{
}

User::~User()
{
}

void User::Update()
{
	isAttack_ = false;

	GetMousePoint(&mouseX_, &mouseY_);
	Gun::Update();
	ImGuiInput();

	// オブジェクト選択
	{
		if (Input::IsKeyDown("selectObject"))
		{
			Collision::SetSelectObject(); // 選択
		}
		if (Input::IsKeyDown("deselectObject"))
		{
			Collision::DeselectObject(); // 選択解除
		}
	}

	// 発砲
	if (Input::IsKeyDown("outBullet"))
	{
		if (Gun::OutBullet() == true)
		{
			isAttack_ = true;
			Collision::AttackedObject(Gun::GetAttack());
		}
	}
	
	// 照準の当たり判定
	{
		VECTOR ScreenPosition = { (float)mouseX_, (float)mouseY_, 1.0f };
		wPointerPosition_ = ConvScreenPosToWorldPos(ScreenPosition);
		startPosition_ = transform_.position_ + LOOK_HEIGHT;

		if (ToolMaster::IsBulletHit(startPosition_, wPointerPosition_) == true)
		{
			isHit_ = true;
		}
		else
		{
			isHit_ = false;
		}
	}

	// 当たり判定
	{
		VECTOR3* pushBack = nullptr;
		if (collider_->CheckOnGround(pushBack) == true)
		{
			int a = 0;
		}
	}

	ToolMaster::CheckSetPosition(this, &velocityY_, distanceR_, gravity_);

	camera_->SetUserPosition(transform_);

	transform_.MakeLocalMatrix();
	MV1SetMatrix(hModel_, transform_.GetLocalMatrix());
	MV1RefreshCollInfo(hModel_);
}

void User::Draw()
{
	Object3D::Draw();
	// 照準の描画
	if (isHit_ == true)
	{
		DrawGraph(mouseX_ - hitAiming_.halfWidth, mouseY_ - hitAiming_.halfHeight, hitAiming_.hImage, TRUE); // Actorに当たる
	}
	else
	{
		DrawGraph(mouseX_ - aiming_.halfWidth, mouseY_ - aiming_.halfHeight, aiming_.hImage, TRUE); // 標準
	}

	
}

void User::ImGuiInput()
{
	// 入力回転
	{
		if (Input::IsKeyKeepDown("rotateRight"))
		{
			transform_.rotation_.y += rotateSpeed_ * DegToRad;
		}
		if (Input::IsKeyKeepDown("rotateLeft"))
		{
			transform_.rotation_.y -= rotateSpeed_ * DegToRad;
		}
	}

	// 入力移動
	{
		VECTOR3 velocity;// 移動ベクトル　velocity→進行方向
		velocity = VECTOR3(0, 0, 1) * moveSpeed_ * MGetRotY(transform_.rotation_.y);//移動方向書いた後、移動距離、回転行列

		if (Input::IsKeyKeepDown("moveFront"))
		{
			transform_.position_ += velocity;
		}
		else if (Input::IsKeyKeepDown("moveBack"))
		{
			transform_.position_ -= velocity;
		}
	}

	Transform t = transform_;

	ImGui::Begin("User");
	ImGui::Text("position");
	float p[3] = { t.position_.x, t.position_.y, t.position_.z };
	ImGui::SliderFloat3("position", p, 0.0f, 10000.0f);

	ImGui::Text("rotation");
	float r[3] = { t.rotation_.x, t.rotation_.y, t.rotation_.z };
	ImGui::SliderFloat3("rotation", r, -DX_PI_F, DX_PI_F);

	ImGui::End();

	transform_.position_ = VECTOR3(p[0], p[1], p[2]);
	transform_.rotation_ = VECTOR3(r[0], r[1], r[2]);

	Gun::ImGuiInput();
}

void User::SetAimingImage(image& i, std::string path)
{
	i.hImage = LoadGraph(path.c_str());
	assert(i.hImage > 0);
	GetGraphSize(i.hImage, &i.halfWidth, &i.halfHeight);
	i.halfWidth = i.halfWidth / 2;
	i.halfHeight = i.halfHeight / 2;
}
