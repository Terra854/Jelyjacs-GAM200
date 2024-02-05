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
	bool IsRGizmoClicked(ImVec2 mousePos);
	void SetRGizmoActive(bool b) { RGizmoActive = b; }
	bool IsRGizmoActive() { return RGizmoActive; }
private:
	Transform* selectedObject;
	Transform XGizmo, YGizmo;
	float R_Radius = 100.f, R_Thickness = 25.f;
	GizmoType type = GizmoType::Translate;
	bool RGizmoActive = false;
};

extern Gizmo gizmo;