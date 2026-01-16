#include "User.h"
#include <assert.h>
#include "Camera.h"
#include "../../ImGui/imgui.h"

namespace USER
{
	const float G = 0.05;	// 重力
	float ROTATE_SPEED = 3.0f;	// 回転速度
	float MOVE_SPEED = 5.0f;	// 移動速度

	const VECTOR3 CAPSULE_POS1 = { 0.0f,  50.0f, 0.0f };
	const VECTOR3 CAPSULE_POS2 = { 0.0f, 150.0f, 0.0f };
	const float DISTANCE_R = 100.0f;

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
	//SetAimingImage(aiming_, "data/image/pointer1.png");
	//SetAimingImage(hitAiming_, "data/image/pointer2.png");
	//SetAimingImage(reload_, "data/image/reload.png");

	rotateSpeed_ = USER::ROTATE_SPEED;
	moveSpeed_ = USER::MOVE_SPEED;

	camera_ = new Camera();
}

User::User()
{
}

User::~User()
{
}

void User::Update()
{
	GetMousePoint(&mouseX_, &mouseY_);
	ImGuiInput();

	camera_->SetUserPosition(transform_);

	transform_.MakeLocalMatrix();
	MV1SetMatrix(hModel_, transform_.GetLocalMatrix());
	MV1RefreshCollInfo(hModel_);
}

void User::Draw()
{
}

void User::ImGuiInput()
{
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
}
