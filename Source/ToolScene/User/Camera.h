#pragma once
#include "../../MyLibrary/Object3D.h"

enum CAM_STATE
{
	FIRST,	// 一人称視点
	THIRD,	// 三人称視点
	FIX,	// 固定カメラ
	MAX_CAM_STATE
};

class Camera : public Object3D
{
public:
	Camera();
	~Camera();
	void Update() override;
	void SetUserPosition(const Transform& transform);

private:
	void FirstUpdate();
	void ThirdUpdate();
	void FixUpdate();
	void ChangeCamera();
	void ImGuiInput(); 

	Transform user_;	// プレイヤーの情報
	int mouseX_;		// マウスのx座標
	int mouseY_;		// マウスのy座標
	int prevMouseX_;	// 前回のマウスのx座標
	int prevMouseY_;	// 前回のマウスのy座標

	CAM_STATE state_;	// カメラの状態

	VECTOR3 cameraPosition_; // 視点の位置
	VECTOR3 targetPosition_; // 注視点の位置
	VECTOR3 fixAddPosition_; // 固定カメラを移動させるときに使用する、加算された位置

	int wheelRot_; // マウスホイールの回転量
};

