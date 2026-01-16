#pragma once
#include "../../MyLibrary/Object3D.h"

namespace Enemy
{
	enum E_SORT {
		STONE,
		TOUHU,
		MAX_E_SORT
	};

	enum E_STATE {
		STAY,
		WALK,
		ATTACK,
		MAX_STATE
	};

	void Init(); // 初期化

	void DevelopmentInput(Transform& t); // 開発時のみ使用する入力処理
	VECTOR3 GetMoveToPlayerPosition(VECTOR3 position); // プレイヤーの方へ移動する
	void CreateEnemy(int enemyNumber, const std::string& fileName, const Transform& t, int hp, int score);

	// 状態の更新処理
	void StayUpdate(int sortNumber, VECTOR3 ePosition, E_STATE* state, E_STATE nextState);
	void WalkUpdate(int sortNumber, bool* isArrive, 
		VECTOR3* goPosition, VECTOR3* ePosition, 
		E_STATE* state, E_STATE nextState);
};

