#pragma once
#include "Object3D.h"

namespace Observer
{
	void SetAttackPower(int power);				// UŒ‚—Í‚ğ‘ã“ü‚·‚é
	int GetAttackPower();						// e’e‚ÌUŒ‚—Í‚ğæ“¾‚·‚é
	void SetHitPosition(VECTOR3 hitPos);		// e’e‚Æƒ‚ƒfƒ‹‚Ì“–‚½‚Á‚½ŒÂŠ‚ğ‘ã“ü‚·‚é
	VECTOR3 GetHitPosition();					// e’e‚Æƒ‚ƒfƒ‹‚Ì“–‚½‚Á‚½ŒÂŠ‚ğæ“¾‚·‚é
	void SetPowerDirection(VECTOR3 powerDir);	// e’e‚Ì•ûŒü‚ÌƒxƒNƒgƒ‹‚ğ‘ã“ü‚·‚é
	VECTOR3 GetPowerDirection();				// e’e‚Ì•ûŒü‚ÌƒxƒNƒgƒ‹‚ğæ“¾‚·‚é
}
