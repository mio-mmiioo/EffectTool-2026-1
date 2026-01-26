#include "Effect.h"
#include <map>
#include "../../ImGui/imgui.h"
#include "../MyLibrary/Observer.h"
#include "Collision.h"

namespace Effect
{
	enum EFFECT_STATE
	{
		ES_CREATING,
		ES_SCALING,
		ES_ROTATING1,
		ES_BOUND1,		// 反発係数が1
		ES_BOUND2,		// 反発係数が徐々に減っていく
		ES_BOUND3,		// 地面にあるボールに上から力を加えた感じの運動
		ES_ATTACKED,
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
	float v0; // 初速度
	float isBounded; // 一回しかバウンドのエフェクトをしたくないときに使用

	// Attack関連
	float attackPower; // 攻撃力、今回の計算では質量とみる
	float speed; // 銃弾の飛ぶ速さ
	float energy; // 運動エネルギー

	float timer; // 各エフェクトにかかる時間を代入するための変数

}

void Effect::Init()
{
	timer = 0.0f;
	// 各エフェクトにかかる時間の代入
	{
		eTime[ES_SCALING] = 0.5f;
		eTime[ES_ROTATING1] = 100.0f;
		eTime[ES_BOUND1] = 4.0f;
		eTime[ES_BOUND2] = 10.0f;
		eTime[ES_BOUND3] = 20.0f;
		eTime[ES_ATTACKED] = 10.0f;
	}

	state = EFFECT_STATE::ES_SCALING;
	effectState = state;
	prevEffectState = effectState;

	gravity = 0.05f;
	velocityY = 0.0f;
	elasticity = 1.0f;
	v0 = 5.0f;
	isBounded = false;

	attackPower = 2; // kg
	speed = 340; // m/s
	energy = (attackPower * speed * speed) / 2.0f;
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
	case EFFECT_STATE::ES_ROTATING1:
		Rotating1(obj);
		break;
	case EFFECT_STATE::ES_BOUND1:
		Bound1(obj);
		break;
	case EFFECT_STATE::ES_BOUND2:
		Bound2(obj);
		break;
	case EFFECT_STATE::ES_BOUND3:
		Bound3(obj);
		break;
	case EFFECT_STATE::ES_ATTACKED:
		Attacked(obj);
		break;
	}

	timer -= Time::DeltaTime();
}

void Effect::Creating(Object3D* obj)
{
	// 今回作成中の効果
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

void Effect::Rotating1(Object3D* obj)
{
	Transform t = obj->GetTransform();
	t.rotation_.y += 0.05f;
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

void Effect::Bound3(Object3D* obj)
{
	Collision::AddVelocity(obj, &velocityY, gravity);
	if (Collision::SetOnGround(obj) == true)
	{
		if (isBounded == false)
		{
			velocityY = v0;
			isBounded = true;
		}
		else
		{
			velocityY = -elasticity * velocityY;
			elasticity -= 0.1f;
			if (elasticity < 0.0f)
			{
				elasticity = 0.0f;
			}
		}
	}
	if (velocityY < -0.3f && velocityY > 0.3f)
	{
		Rotating1(obj);
	}

	if (elasticity == 0.0f && timer <= 0.0f)
	{
		isBounded = false;
	}

}

void Effect::Attacked(Object3D* obj)
{
	Transform t = obj->GetTransform();
	VECTOR3 h = Observer::GetHitPosition();
	VECTOR3 d = Observer::GetPowerDirection();

	// オブジェクトの位置と銃弾がヒットする位置
	// この2点の距離をもとに
	energy = (attackPower * speed * speed) / 2.0f; // 運動エネルギー

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
		elasticity = 1.0f;
	}

	// ラジオボタンによる選択
	ImGui::RadioButton("No Effect", &effectState, EFFECT_STATE::MAX_EFFECT_STATE);
	ImGui::RadioButton("Creating", &effectState, EFFECT_STATE::ES_CREATING);
	ImGui::RadioButton("Rotating1", &effectState, EFFECT_STATE::ES_ROTATING1);
	ImGui::RadioButton("Scaling", &effectState, EFFECT_STATE::ES_SCALING);
	ImGui::RadioButton("Bound1", &effectState, EFFECT_STATE::ES_BOUND1);
	ImGui::RadioButton("Bound2", &effectState, EFFECT_STATE::ES_BOUND2);
	ImGui::RadioButton("Bound3", &effectState, EFFECT_STATE::ES_BOUND3);
	ImGui::RadioButton("Attackd", &effectState, EFFECT_STATE::ES_ATTACKED);


	VECTOR3 p = Observer::GetHitPosition();
	VECTOR3 d = Observer::GetPowerDirection();
	attackPower = Observer::GetAttackPower();

	switch (effectState)
	{
	case EFFECT_STATE::ES_CREATING:
		state = EFFECT_STATE::ES_CREATING;
		ImGui::Begin("Creating");
		ImGui::End();
		break;
	case EFFECT_STATE::ES_SCALING:
		state = EFFECT_STATE::ES_SCALING;
		break;
	case EFFECT_STATE::ES_ROTATING1:
		state = EFFECT_STATE::ES_ROTATING1;
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
	case EFFECT_STATE::ES_BOUND3:
		state = EFFECT_STATE::ES_BOUND3;
		ImGui::Begin("Bound3");
		ImGui::InputFloat("v0", &v0);
		ImGui::InputFloat("velocity", &velocityY);
		ImGui::InputFloat("elasticity", &elasticity);
		ImGui::End();
		break;
	case EFFECT_STATE::ES_ATTACKED:
		state = EFFECT_STATE::ES_ATTACKED;
		ImGui::Begin("Attack");
		ImGui::InputFloat("Attack Power", &attackPower);
		ImGui::Text("energy : %f", energy);
		ImGui::Text("hitPosition   : (%04f, %04f, %04f)", p.x, p.y, p.z);
		ImGui::Text("PowerDirection: (%04f, %04f, %04f)", d.x, d.y, d.z);
		ImGui::End();
		break;
	case EFFECT_STATE::MAX_EFFECT_STATE:
		state = EFFECT_STATE::MAX_EFFECT_STATE;
		break;
	}
}
