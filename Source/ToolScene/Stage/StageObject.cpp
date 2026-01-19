#include "StageObject.h"
#include <assert.h>
#include "../Collision.h"
#include "../../../ImGui/imgui.h"

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

	// この下でエフェクトをいじる
	ImGui::Begin("SelectdObject");
	float p[3] = { transform_.position_.x, transform_.position_.y, transform_.position_.z };
	//ImGui::InputFloat3("position", &p[3], "%.3f");
	ImGui::Text("position");
	ImGui::InputFloat("x:", &transform_.position_.x);
	ImGui::InputFloat("y:", &transform_.position_.y);
	ImGui::InputFloat("z:", &transform_.position_.z);
	ImGui::End();


	transform_.MakeLocalMatrix();
	MV1SetMatrix(hModel_, transform_.GetLocalMatrix());
	MV1RefreshCollInfo(hModel_);
}