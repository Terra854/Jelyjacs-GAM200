#pragma once
#include <Vec2.h>
#include <components/Transform.h>

enum class GizmoType {
	Scale,
	Rotate,
	Translate
};

class Gizmo {
public:
	void SetObject(Transform* t);
	void RenderGizmo();
	Transform* getX() { return &XGizmo; }
	Transform* getY() { return &YGizmo; }
	void SetType(GizmoType g) { type = g; }
private:
	Transform* selectedObject;
	Transform XGizmo, YGizmo;
	GizmoType type = GizmoType::Translate;
};

extern Gizmo gizmo;