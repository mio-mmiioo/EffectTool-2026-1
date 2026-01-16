#pragma once
#include "../../MyLibrary/Object3D.h"
#include "Enemy.h"

class Touhu : public Object3D
{
public:
	Touhu(const std::string& fileName, const Transform& t, int hp, int score);
	~Touhu();
	void Update() override;
	void Draw() override;

private:
	void DevelopmentInput(); // ŠJ”­‚Ì‚İg—p‚·‚éˆ—

	Enemy::E_STATE state_; // ó‘Ô
	float stayTimer_; // ó‘Ô‘JˆÚ‚·‚é‚Ü‚Å‚ÌŠÔ
	bool isArrive_; // “’…‚µ‚½‚© true¨“’…‚µ‚½
	VECTOR3 goPosition_; // Ÿ‚ÉŒü‚©‚¤êŠ
};
