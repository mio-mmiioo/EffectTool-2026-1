#pragma once
#include "../../MyLibrary/Object3D.h"

class Camera;

struct image
{
	int hImage;		// 画像のハンドル
	int halfWidth;	// 画像の半分の横幅
	int halfHeight; // 画像の半分の縦幅
};

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
	void SetAimingImage(image& i, std::string path);

	Camera* camera_;

	int mouseX_;				// マウスのx座標
	int mouseY_;				// マウスのy座標
	VECTOR3 wPointerPosition_;	// スクリーン座標の照準をワールド座標に変換した座標
	VECTOR3 startPosition_;		// 銃の軌道の始点

	bool isHit_;	// 銃弾が当たるか true→当たる
	bool isAttack_; // 攻撃（発砲）しているか

	// 照準の画像関連
	image aiming_;		// 照準の画像
	image hitAiming_;	// 照準が何かにあたるときの画像
};