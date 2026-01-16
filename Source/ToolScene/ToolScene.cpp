#include "ToolScene.h"
#include "ToolMaster.h"

ToolScene::ToolScene()
{
	ToolMaster::Init();
}

ToolScene::~ToolScene()
{
	ToolMaster::Release();
}

void ToolScene::Update()
{
	ToolMaster::Update();
}

void ToolScene::Draw()
{
	ToolMaster::Draw();
}