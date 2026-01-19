#include "Effect.h"
#include <map>
#include "../../ImGui/imgui.h"


namespace Effect
{
	enum EFFECT_STATE
	{
		ES_SCALING,
		ES_BOUND,
		MAX_EFFECT_STATE
	};

	EFFECT_STATE state;

	std::map<EFFECT_STATE, float> eTime; // 各エフェクトの時間

	static int effectState; // 現在のエフェクトの状態
	static int prevEffectState; // 前回のエフェクトの状態

	bool isStartEffect = false; // エフェクトを開始するか

	float timer;
}

void Effect::Init()
{
	timer = 0.0f;
	// 各エフェクトにかかる時間の代入
	{
		eTime[ES_SCALING] = 1.0f;
		eTime[ES_BOUND] = 4.0f;
	}


	state = EFFECT_STATE::ES_SCALING;
	effectState = state;
	prevEffectState = effectState;
}

void Effect::Update(Transform* t)
{
	if (timer <= 0.0f) return;

	switch (state)
	{
	case EFFECT_STATE::ES_SCALING:
		if (timer > 0.5)
		{
			t->scale_ = t->scale_ * 1.1f;
		}
		else
		{
			t->scale_ = t->scale_ * 0.9f;
		}
		break;
	}

	timer -= Time::DeltaTime();
}

void Effect::Scaling(Transform* t)
{
	if (timer > eTime[ES_SCALING] / 2)
	{
		t->scale_ = t->scale_ * 1.05f;
	}
	else
	{
		t->scale_ = t->scale_ * 0.95f;
	}
}

void Effect::ImGuiInput()
{
	prevEffectState = effectState;

	if (ImGui::Button("Start Effect") == true && timer <= 0.0f)
	{
		timer = eTime[state];
	}

	//ImGui::SliderFloat("timer", &timer, 0.0f, eTime[state]);

	ImGui::RadioButton("No Effect", &effectState, EFFECT_STATE::MAX_EFFECT_STATE);
	ImGui::RadioButton("Scaling", &effectState, EFFECT_STATE::ES_SCALING);
	ImGui::RadioButton("Bound", &effectState, EFFECT_STATE::ES_BOUND);


	switch (effectState)
	{
	case EFFECT_STATE::ES_SCALING:
		state = EFFECT_STATE::ES_SCALING;
		break;
	case EFFECT_STATE::ES_BOUND:
		state = EFFECT_STATE::ES_BOUND;
		break;
	case EFFECT_STATE::MAX_EFFECT_STATE:
		state = EFFECT_STATE::MAX_EFFECT_STATE;
		break;
	}
}
