#include "StageObject.h"
#include <assert.h>
#include "../Collision.h"
#include "../../../ImGui/imgui.h"
#include "../Effect.h"

namespace STAGE_OBJECT
{
	const float GRAVITY = 0.05f;
}

StageObject::StageObject(int objectNumber, const std::string& fileName, const Transform& t, int hp, int score)
{
	const std::string folder = "data/model/";
	hModel_ = MV1LoadModel((folder + fileName + ".mv1").c_str());
	assert(hModel_ > 0);
	hitModel_ = MV1LoadModel((folder + fileName + "_c.mv1").c_str());
	assert(hitModel_ > 0);

	transform_ = t;
	transform_.MakeLocalMatrix();
	MV1SetMatrix(hitModel_, transform_.GetLocalMatrix());
	MV1SetupCollInfo(hitModel_);

	hp_ = hp;
	objectNumber_ = objectNumber;
	score_ = score;
	isSelect_ = false;

	if (hp_ <= 0)
	{
		isDestructible_ = false;
	}
	else
	{
		isDestructible_ = true;
	}

	Collision::AddObject(this);

	// ツールとして必要なものの初期化
	isGravity_ = false;
	velocityY_ = 0.0f;
}

StageObject::~StageObject()
{
	if (hModel_ > 0) {
		MV1DeleteModel(hModel_);
		hModel_ = -1;
	}
	if (hitModel_ > 0) {
		MV1DeleteModel(hitModel_);

		hitModel_ = -1;
	}
}

void StageObject::Update()
{
	if (isDestructible_ == true && hp_ <= 0)
	{
		DestroyMe();
		return;
	}

	if (isSelect_ == false)
	{
		return;
	}

	ImGuiInput();
	// この下でエフェクトをいじる
	
	if (isGravity_ == true)
	{
		Collision::SetOnGround(this, &velocityY_, STAGE_OBJECT::GRAVITY);
	}


	transform_.MakeLocalMatrix();
	MV1SetMatrix(hModel_, transform_.GetLocalMatrix());
	MV1RefreshCollInfo(hModel_);
}

void StageObject::ImGuiInput()
{
	ImGui::Begin("SelectdObject");
	
	// transform
	{
		ImGui::Text("position");
		ImGui::InputFloat("position x:", &transform_.position_.x);
		ImGui::InputFloat("position y:", &transform_.position_.y);
		ImGui::InputFloat("position z:", &transform_.position_.z);

		ImGui::Text("rotation");
		ImGui::InputFloat("rotation x:", &transform_.rotation_.x);
		ImGui::InputFloat("rotation y:", &transform_.rotation_.y);
		ImGui::InputFloat("rotation z:", &transform_.rotation_.z);

		ImGui::Text("scale");
		ImGui::InputFloat("scale x:", &transform_.scale_.x);
		ImGui::InputFloat("scale y:", &transform_.scale_.y);
		ImGui::InputFloat("scale z:", &transform_.scale_.z);
	}

	ImGui::Checkbox("gravity", &isGravity_);

	Effect::ImGuiInput();

	ImGui::End();
}