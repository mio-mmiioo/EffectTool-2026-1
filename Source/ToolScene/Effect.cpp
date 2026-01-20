#include "Effect.h"
#include <map>
#include "../../ImGui/imgui.h"
#include "Collision.h"

namespace Effect
{
	enum EFFECT_STATE
	{
		ES_SCALING,
		ES_BOUND1,
		ES_BOUND2,
		MAX_EFFECT_STATE
	};

	EFFECT_STATE state;

	std::map<EFFECT_STATE, float> eTime; // 各エフェクトの時間

	static int effectState; // 現在のエフェクトの状態
	static int prevEffectState; // 前回のエフェクトの状態

	bool isStartEffect = false; // エフェクトを開始するか
	bool isRepeat = false; // リピートをするか true → リピートON

	// bound関連
	float gravity; // 重力
	float velocityY; // 
	float elasticity; // 反発係数

	float timer; // 各エフェクトにかかる時間を代入するための変数

}

void Effect::Init()
{
	timer = 0.0f;
	// 各エフェクトにかかる時間の代入
	{
		eTime[ES_SCALING] = 0.5f;
		eTime[ES_BOUND1] = 4.0f;
		eTime[ES_BOUND2] = 10.0f;
	}

	state = EFFECT_STATE::ES_SCALING;
	effectState = state;
	prevEffectState = effectState;

	gravity = 0.05f;
	velocityY = 0.0f;
	elasticity = 1.0f;
}

void Effect::Update(Object3D* obj)
{
	// タイマーが0以下だった場合の処理
	if (timer <= 0.0f)
	{
		if (isStartEffect == true)
		{
			if (isRepeat == true)
			{
				timer = eTime[state];
			}
			else
			{
				isStartEffect = false;
			}
		}
		else
		{
			return;
		}
	}

	switch (state)
	{
	case EFFECT_STATE::ES_SCALING:
		Scaling(obj);
		break;
	case EFFECT_STATE::ES_BOUND1:
		Bound1(obj);
		break;
	case EFFECT_STATE::ES_BOUND2:
		Bound2(obj);
		break;
	}

	timer -= Time::DeltaTime();
}

void Effect::Scaling(Object3D* obj)
{
	Transform t = obj->GetTransform();
	if (timer == eTime[ES_SCALING])
	{
		t.scale_ = t.scale_ * 0.7f;
	}

	if (timer > 0.0f)
	{
		t.scale_ = t.scale_ * 1.01f;
	}
	obj->SetTransform(t);
}

void Effect::Bound1(Object3D* obj)
{
	Collision::AddVelocity(obj, &velocityY, gravity);
	if (Collision::SetOnGround(obj) == true)
	{
		velocityY = -elasticity * velocityY;
	}
}

void Effect::Bound2(Object3D* obj)
{
	Collision::AddVelocity(obj, &velocityY, gravity);
	if (Collision::SetOnGround(obj) == true)
	{
		velocityY = -elasticity * velocityY;
		elasticity -= 0.1f;
		if (elasticity < 0.0f)
		{
			elasticity = 0.0f;
		}
	}
}

void Effect::ImGuiInput()
{
	prevEffectState = effectState;

	ImGui::Text("effect timer : %f", timer);
	ImGui::Checkbox("Repeat", &isRepeat);
	if (ImGui::Button("Start Effect") == true && timer <= 0.0f)
	{
		isStartEffect = true;
		timer = eTime[state];
	}

	//ImGui::SliderFloat("timer", &timer, 0.0f, eTime[state]);

	ImGui::RadioButton("No Effect", &effectState, EFFECT_STATE::MAX_EFFECT_STATE);
	ImGui::RadioButton("Scaling", &effectState, EFFECT_STATE::ES_SCALING);
	ImGui::RadioButton("Bound1", &effectState, EFFECT_STATE::ES_BOUND1);
	ImGui::RadioButton("Bound2", &effectState, EFFECT_STATE::ES_BOUND2);

	switch (effectState)
	{
	case EFFECT_STATE::ES_SCALING:
		state = EFFECT_STATE::ES_SCALING;
		break;
	case EFFECT_STATE::ES_BOUND1:
		state = EFFECT_STATE::ES_BOUND1;
		break;
	case EFFECT_STATE::ES_BOUND2:
		state = EFFECT_STATE::ES_BOUND2;
		ImGui::Begin("Bound2");
		ImGui::InputFloat("velocity", &velocityY);
		ImGui::InputFloat("elasticity", &elasticity);
		ImGui::End();
		break;
	case EFFECT_STATE::MAX_EFFECT_STATE:
		state = EFFECT_STATE::MAX_EFFECT_STATE;
		break;
	}
}
