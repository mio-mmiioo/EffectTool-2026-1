#pragma once
#include "../../MyLibrary/Object3D.h"

class StageObject : public Object3D
{
public:
	StageObject(int objectNumber, const std::string& fileName, const Transform& t, int hp, int score);
	~StageObject();

	void Update() override;
	void SetSelect(bool flag) { isSelect_ = flag; }

private:
	bool isDestructible_; // ‰ó‚ê‚é‚à‚Ì‚© true¨ ‰ó‚ê‚é
	bool isSelect_; // ‘I‘ğ‚³‚ê‚Ä‚¢‚é‚© true ¨ ‘I‘ğ‚³‚ê‚Ä‚¢‚é
	int score_; // “¾“_
};