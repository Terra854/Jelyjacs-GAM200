#include "Gizmo.h"
#include <Mat3.h>
#include <GL/glew.h> // for access to OpenGL API declarations 
#include <GLFW/glfw3.h>
#include <Camera.h>
#include "graphic_gl/glapp.h"

glm::vec3 x_gizmo_color{ 1.0f, 0.0f, 0.0f };
glm::vec3 y_gizmo_color{ 0.0f, 1.0f, 0.0f };

Gizmo gizmo;

void Gizmo::SetObject(Transform* t){
	selectedObject = t;

}

void Gizmo::RenderGizmo(){

	Vec2 gizmoPos, gizmoXPos, gizmoYPos, gizmoXScaling, gizmoYScaling;

	//gizmoPos = Vec2(selectedObject->Position.x * camera2D->scale.x, selectedObject->Position.y * camera2D->scale.y);
	gizmoPos = Vec2(selectedObject->Position.x, selectedObject->Position.y);

	XGizmo.Position = gizmoPos + Vec2(72.f / camera2D->scale.x, 0.f);
	XGizmo.Rotation = 0.f;
	XGizmo.Scale = Vec2(128.f, 16.f) / camera2D->scale.x;

	YGizmo.Position = gizmoPos + Vec2(0.f, 72.f / camera2D->scale.y);
	YGizmo.Rotation = 0.f;
	YGizmo.Scale = Vec2(16.f, 128.f) / camera2D->scale.x;

	/* Gizmo */
	Mat3 gizmoXMat, gizmoYMat;

	// get pos and scale from transform component

	gizmoXPos = Vec2(XGizmo.Position.x * 2.0f / window->width, XGizmo.Position.y * 2.0f / window->height);
	gizmoYPos = Vec2(YGizmo.Position.x * 2.0f / window->width, YGizmo.Position.y * 2.0f / window->height);

	gizmoXScaling = Vec2(XGizmo.Scale.x / window->width, XGizmo.Scale.y / window->height);
	gizmoYScaling = Vec2(YGizmo.Scale.x / window->width, YGizmo.Scale.y / window->height);

	gizmoXMat = Mat3Translate(gizmoXPos) * Mat3Scale(gizmoXScaling) * Mat3RotDeg(0);
	gizmoYMat = Mat3Translate(gizmoYPos) * Mat3Scale(gizmoYScaling) * Mat3RotDeg(0);

	gizmoXMat = camera2D->world_to_ndc * gizmoXMat;
	gizmoYMat = camera2D->world_to_ndc * gizmoYMat;

	// matrix after camrea

	//Mat3 gizmoCam = Mat3Scale(1.f, 1.f) * Mat3Translate(camera2D->position.x * camera2D->scale.x, camera2D->position.y * camera2D->scale.y);

	//gizmoXMat = gizmoCam * gizmoXMat;
	//gizmoYMat = gizmoCam * gizmoYMat;

	GLApp::shdrpgms["shape"].Use();
	// bind VAO of this object's model
	glBindVertexArray(GLApp::models["square"].vaoid);

	// Render X arrow
	GLApp::shdrpgms["shape"].SetUniform("uModel_to_NDC", gizmoXMat.ToGlmMat3());
	GLApp::shdrpgms["shape"].SetUniform("uColor", x_gizmo_color);
	glDrawElements(GLApp::models["square"].primitive_type, GLApp::models["square"].draw_cnt, GL_UNSIGNED_SHORT, 0);

	// Render Y arrow
	GLApp::shdrpgms["shape"].SetUniform("uModel_to_NDC", gizmoYMat.ToGlmMat3());
	GLApp::shdrpgms["shape"].SetUniform("uColor", y_gizmo_color);
	glDrawElements(GLApp::models["square"].primitive_type, GLApp::models["square"].draw_cnt, GL_UNSIGNED_SHORT, 0);

	// unbind VAO and unload shader program
	glBindVertexArray(0);
	GLApp::shdrpgms["shape"].UnUse();
}