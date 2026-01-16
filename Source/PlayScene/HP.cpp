#include "HP.h"
#include <assert.h>

namespace {
	const VECTOR2 LEFT_TOP = { 20, 20 }; // 左上の座標
	const int IMAGE_WIDTH = 400; // 画像のサイズ
	const int IMAGE_HEIGHT = 43;
	const float DAMAGING_TIME = 0.4f; // 攻撃されている時間
	const float DAMAGED_TIME = 2.0f; // 攻撃された後の時間
}

HP::HP(int hp)
{
	addHp_ = 0;
	hp_ = hp;
	maxHp_ = hp;

	// 画像の読み込み
	hBarFrameImage_ = LoadGraph("data/image/hpBarFrame.png");
	assert(hBarFrameImage_ > 0);
	hBarGreenImage_ = LoadGraph("data/image/hpBarGreen.png");
	assert(hBarGreenImage_ > 0);
	hBarRedImage_ = LoadGraph("data/image/hpBarRed.png");
	assert(hBarRedImage_ > 0);

	timer_ = 0.0f;
	hpRaitio_ = 0.0f;
	state_ = HP_STATE::HP_NORMAL;
}

HP::~HP()
{
}

void HP::Update()
{
	switch (state_)
	{
	case HP_STATE::HP_NORMAL:
		break;
	case HP_STATE::HP_HEAL:
		state_ = HP_STATE::HP_NORMAL;
		break;
	case HP_STATE::HP_DAMAGING:
		timer_ -= Time::DeltaTime();
		if (timer_ <= 0.0f)
		{
			timer_ = DAMAGED_TIME;
			state_ = HP_STATE::HP_DAMAGED;
		}
		break;
	case HP_STATE::HP_DAMAGED:
		timer_ -= Time::DeltaTime();
		if (timer_ <= 0.0f)
		{
			addHp_ = 0;
			hpRaitio_ = 0.0f;
			state_ = HP_STATE::HP_NORMAL;
		}
		break;
	}
}

void HP::Draw()
{
	float raitio = (float)hp_ / (float)maxHp_;
	DrawGraph((int)LEFT_TOP.x, (int)LEFT_TOP.y, hBarFrameImage_, TRUE);

	switch (state_)
	{
	case HP_STATE::HP_NORMAL:
	case HP_STATE::HP_HEAL:
	case HP_STATE::HP_DAMAGING:
		DrawRectGraph((int)LEFT_TOP.x, (int)LEFT_TOP.y, 0, 0, (int)(IMAGE_WIDTH * (raitio + hpRaitio_ * 1.0f)), IMAGE_HEIGHT, hBarRedImage_, TRUE);
		break;
	case HP_STATE::HP_DAMAGED:
		timeRaitio_ = timer_ / DAMAGED_TIME;
		DrawRectGraph((int)LEFT_TOP.x, (int)LEFT_TOP.y, 0, 0, (int)(IMAGE_WIDTH * (raitio + hpRaitio_ * timeRaitio_)), IMAGE_HEIGHT, hBarRedImage_, TRUE);
		break;
	}
	DrawRectGraph((int)LEFT_TOP.x, (int)LEFT_TOP.y, 0, 0, (int)(IMAGE_WIDTH * raitio), IMAGE_HEIGHT, hBarGreenImage_, TRUE);
}

void HP::AddHP(int addHp)
{
	addHp_ = std::abs(addHp);
	hpRaitio_ = (float)std::abs(addHp_) / (float)maxHp_ + hpRaitio_ * timeRaitio_;
	hp_ += addHp;
	if (hp_ > maxHp_) // 最大値より大きくなっていた場合
	{
		hp_ = maxHp_;
	}
	if (addHp > 0)
	{
		state_ = HP_STATE::HP_HEAL;
	}
	else if (addHp < 0)
	{
		state_ = HP_STATE::HP_DAMAGING;
		timer_ = DAMAGING_TIME;
	}
}
