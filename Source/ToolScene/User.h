#pragma once
#include "../MyLibrary/Object3D.h"

class Camera;

class User : public Object3D
{
public:
	User(const VECTOR3& position);
	User();
	~User();
	void Update() override;
	void Draw() override;

private:
	void ImGuiInput();
	
	Camera* camera_;

	int mouseX_;				// マウスのx座標
	int mouseY_;				// マウスのy座標
	VECTOR3 wPointerPosition_;	// スクリーン座標の照準をワールド座標に変換した座標
	VECTOR3 startPosition_;		// 銃の軌道の始点


};