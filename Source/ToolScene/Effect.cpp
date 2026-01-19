#include "Effect.h"
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

	float scalingTime = 2.0f;


	float time;
}

void Effect::Update()
{

}

void Effect::Scaling(Transform* t)
{

}

void Effect::ImGuiInput()
{
	static int effectState = 0;

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
	}
}
