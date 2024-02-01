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
	GizmoType GetType() { return type; }
private:
	Transform* selectedObject;
	Transform XGizmo, YGizmo;
	float R_Radius = 100.f, R_Thickness = 10.f;
	GizmoType type = GizmoType::Translate;
};

extern Gizmo gizmo;