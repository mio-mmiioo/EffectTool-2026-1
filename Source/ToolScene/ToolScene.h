#pragma once
#include "../../Library/SceneBase.h"

class ToolScene : public SceneBase
{
public:
	ToolScene();
	~ToolScene();
	void Update() override;
	void Draw() override;
};