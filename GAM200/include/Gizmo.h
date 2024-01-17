#pragma once
#include <Vec2.h>
#include <components/Transform.h>

class Gizmo {
public:
	void SetObject(Transform* t);
	void RenderGizmo();
	Transform* getX() { return &XGizmo; }
	Transform* getY() { return &YGizmo; }
private:
	Transform* selectedObject;
	Transform XGizmo, YGizmo;
};

extern Gizmo gizmo;