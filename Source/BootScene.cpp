#include "BootScene.h"

BootScene::BootScene()
{
}

BootScene::~BootScene()
{
}

void BootScene::Update()
{
	SceneManager::ChangeScene("TOOL");
}

void BootScene::Draw()
{
}
