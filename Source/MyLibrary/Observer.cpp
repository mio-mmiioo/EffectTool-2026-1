#include "Observer.h"

namespace Observer
{
	int attackPower = 0;
	VECTOR3 hitPosition = { 0.0f, -1000.0f, 0.0f };
	VECTOR3 powerDirection = { 0.0f, 0.0f, 0.0f };
}

void Observer::SetAttackPower(int power)
{
	attackPower = power;
}

int Observer::GetAttackPower()
{
	return attackPower;
}

void Observer::SetHitPosition(VECTOR3 hitPos)
{
	hitPosition = hitPos;
}

VECTOR3 Observer::GetHitPosition()
{
	return hitPosition;
}

void Observer::SetPowerDirection(VECTOR3 powerDir)
{
	powerDirection = powerDir;
}

VECTOR3 Observer::GetPowerDirection()
{
	return powerDirection;
}
