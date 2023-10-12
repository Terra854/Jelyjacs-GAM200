#include <Camera.h>
#include <iostream>

// globel camera
Camera* camera2D = NULL;
Camera::Camera() {
	camera2D = this;
}


void Camera::Initialize() {
	scale = { 1.f,1.f};
}

void Camera::Update() {
	// loop through all objects and find the player
	for (long i = 0; i < (long)objectFactory->NumberOfObjects(); i++) {
		// if the object is a player
		if (static_cast<PlayerControllable*>((objectFactory->getObjectWithID(i))->GetComponent(ComponentType::PlayerControllable)) != nullptr) {
			// get the player's position
			Transform* trans = static_cast<Transform*>((objectFactory->getObjectWithID(i))->GetComponent(ComponentType::Transform));
			
			position.x = 0.f - trans->Position.x * 2.0f / window->width;
			
			position.y = 0.f - trans->Position.y * 2.0f / window->height;
			// make sure the camera is not out of bounds
			if(position.x > 1.f - 1.f/scale.x) {
				position.x = 1.f - 1.f/scale.x;
			}
			else if (position.x < -1.f + 1.f / scale.x) {
				position.x = -1.f + 1.f / scale.x;
			}
			
			if(position.y > 1.f - 1.f / scale.y) {
				position.y = 1.f - 1.f / scale.y;
			}
			else if (position.y < -1.f + 1.f / scale.y) {
				position.y = -1.f + 1.f / scale.y;
			}
		}
	}
	
	world_to_ndc = Mat3Scale(scale.x, scale.y)*Mat3Translate(position.x, position.y);

}

