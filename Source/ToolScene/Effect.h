#pragma once
#include "../MyLibrary/Object3D.h"

namespace Effect
{
	void Init();
	void Update(Object3D* obj);
	void Scaling(Transform* t);
	void Bound(Object3D* obj);
	void ImGuiInput();
}
