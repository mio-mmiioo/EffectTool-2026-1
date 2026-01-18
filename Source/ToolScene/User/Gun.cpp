#include "Gun.h"
#include "../../../ImGui/imgui.h"
#include "../../../Library/Time.h"

namespace Gun
{
	const int ATTACK = 2;
	const float COOL_DOWN_TIME = 0.7f;
	int attack;						// 攻撃力
	float coolDownTimer;			// 連射を防ぐためのタイマー
	float coolDownTime;				// 次に撃てるようになるまでの時間
}

void Gun::Init()
{
	attack = ATTACK;
	coolDownTimer = 0.0f;
	coolDownTime = COOL_DOWN_TIME;
}

void Gun::Update()
{
	if (coolDownTimer > 0.0f)
	{
		coolDownTimer -= Time::DeltaTime();
	}
}

void Gun::Release()
{

}

bool Gun::OutBullet()
{
	if (coolDownTimer > 0.0f) // 残弾数が0以下 or クールダウン中
	{
		return false;
	}

	coolDownTimer = coolDownTime;
	
	//PlaySoundMem(Sound::se["OutBullet1"], DX_PLAYTYPE_BACK, TRUE);
	//OutBulletEffect();
	return true;
}

int Gun::GetAttack()
{
	return -attack;
}

void Gun::ImGuiInput()
{
	ImGui::Begin("Gun");
	ImGui::InputInt("Power", &attack);
	ImGui::InputFloat("CoolDownTime", &coolDownTimer);
	ImGui::End();
}
