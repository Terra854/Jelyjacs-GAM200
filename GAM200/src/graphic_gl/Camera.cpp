#include <Camera.h>
#include <iostream>
#include <input.h>

// globel camera
Camera* camera2D = NULL;
Camera::Camera() {
	camera2D = this;
}


void Camera::Initialize() {
	scale = { 1.f,1.f };
}

void Camera::Update() {

	if (!isFreeCamEnabled()) {

		// Set scale for camera

		if (input::IsPressed(KEY::c)) {
			if (camera2D->scale.x == 1.0f || camera2D->scale.y == 1.0f) {
				camera2D->scale = { 2.0f, 2.0f };
			}
			else {
				position = { 0.0f, 0.0f };
				camera2D->scale = { 1.0f, 1.0f };
			}
		}

		SetToPlayer();

	}


	world_to_ndc = Mat3Scale(scale.x, scale.y) * Mat3Translate(position.x, position.y);

}

void Camera::SetToPlayer() {
	// get player object
	Object* player = objectFactory->getPlayerObject();

	// Make sure it's not nullptr
	if (player != nullptr) {

		// get the player's position
		if (scale.x != 1.f || scale.y != 1.f) {
			Transform* trans = static_cast<Transform*>(player->GetComponent(ComponentType::Transform));

			position.x = 0.f - trans->Position.x * 2.0f / window->width;

			position.y = 0.f - trans->Position.y * 2.0f / window->height;
			// make sure the camera is not out of bounds

			if (position.x > 0.6f - 1.f / scale.x) {
				position.x = 0.6f - 1.f / scale.x;
			}
			else if (position.x < -0.6f + 1.f / scale.x) {
				position.x = -0.6f + 1.f / scale.x;
			}

			if (position.y > 0.8f - 1.f / scale.y) {
				position.y = 0.8f - 1.f / scale.y;
			}
			else if (position.y < -0.8f + 1.f / scale.y) {
				position.y = -0.8f + 1.f / scale.y;
			}
		}

	}
}