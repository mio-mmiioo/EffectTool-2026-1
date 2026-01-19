#pragma once
#include "../../MyLibrary/Object3D.h"

class StageObject : public Object3D
{
public:
	StageObject(int objectNumber, const std::string& fileName, const Transform& t, int hp, int score);
	~StageObject();

	void Update() override;

private:
	void ImGuiInput();
	bool isDestructible_; // 壊れるものか true → 壊れる
	int score_; // 得点

	// ツールとして必要なもの
	bool isGravity_; // 重力を加えるか true → 使用する

};