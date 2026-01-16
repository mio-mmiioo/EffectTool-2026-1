#pragma once
#include "../MyLibrary/Object2D.h"

class HP : public Object2D
{
public:
	HP(int hp);
	~HP();
	void Update() override;
	void Draw() override;
	int GetHP() { return hp_; }
	void AddHP(int addHp);

private:
	enum HP_STATE {
		HP_NORMAL,		// 通常
		HP_HEAL,		// 回復している
		HP_DAMAGING,	// 攻撃されている
		HP_DAMAGED,		// 攻撃された
		HP_MAX_STATE
	};

	int addHp_;
	int hp_;
	int maxHp_;

	float timer_;
	float hpRaitio_;
	float timeRaitio_;

	// 画像
	int hBarFrameImage_; // HPゲージの枠
	int hBarGreenImage_; // HPゲージ・緑色
	int hBarRedImage_; // HPゲージ・赤色

	HP_STATE state_;
};