#pragma once
#include "../MyLibrary/Object3D.h"

namespace Effect
{
	void Init();
	void Update(Object3D* obj);
	void Creating(Object3D* obj);
	void Scaling(Object3D* obj);
	void Rotating1(Object3D* obj);
	void Bound1(Object3D* obj);
	void Bound2(Object3D* obj);
	void Bound3(Object3D* obj);
	void Attacked(Object3D*  obj);
	void ImGuiInput();
}
