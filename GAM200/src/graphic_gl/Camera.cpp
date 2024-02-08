/* !
@file    Camera.cpp
@author  Guo Chen (g.chen@digipen.edu)
@date    02/11/2023

Camera system for 2D game.
*//*__________________________________________________________________________*/
#include <Camera.h>
#include <iostream>
#include <input.h>

// globel camera
Camera* camera2D = NULL;
Camera::Camera() {
	camera2D = this;
}

/*  _________________________________________________________________________ */
/*
* Initialize the camera
*/
void Camera::Initialize() {
	scale = { 1.f,1.f };
	position = { 0.f,0.f };
}

/*  _________________________________________________________________________ */
/*
* Update the camera
* Press C to zoom in/out
*/
void Camera::Update() {

	if (!isFreeCamEnabled()) {

		// Set scale for camera

		if (input::IsPressed(KEY::c)) {
			if (camera2D->scale.x == 1.0f || camera2D->scale.y == 1.0f) {
				camera2D->scale = { 2.0f, 2.0f };
			}
			else {
				scale = { 1.0f, 1.0f };
			}
		}
		//if(scale.x==1.0f && scale.y==1.0f)position = { 0.0f, 0.0f };

		SetToPlayer();

	}


	world_to_ndc = Mat3Scale(scale.x, scale.y) * Mat3Translate(position.x, position.y);
	//Vec2 window_scaling = {(float)window->width_init/(float)window->width,(float)window->height_init/(float)window->height};
	//world_to_ndc = Mat3Scale(window_scaling.x, window_scaling.y) * world_to_ndc;

}

/*  _________________________________________________________________________ */
/*
* Set the camera scale
* @param scale_input The scale of the camera
* 
*/
void Camera::SetCameraScale(Vec2 scale_input)
{
	scale.x= scale_input.x/window->width_init;
	scale.y = scale_input.y/window->height_init;
}

/*  _________________________________________________________________________ */
/*
* Set the camera position
* @param position_input The position of the camera
* 
*/
void Camera::SetCameraPosition(Vec2 position_input)
{
	position.x = position_input.x;
	position.y = position_input.y;

}

/*  _________________________________________________________________________ */
/*
* Set the camera to the player's position
*/
void Camera::SetToPlayer() {
	// get player object
	Object* player = Logic->playerObj;

	// Make sure it's not nullptr
	if (player != nullptr) {

		// get the player's position
		Transform* trans = static_cast<Transform*>(player->GetComponent(ComponentType::Transform));

		position.x = trans->Position.x ;

		position.y = trans->Position.y ;
		

		position.x = -position.x * 2.0f / window->width_init;

		position.y = -position.y * 2.0f / window->height_init;
		// make sure the camera is not out of bounds
		if (position.x < x_min ) position.x = x_min;
		if (position.y < y_min) position.y = y_min;
		if (position.x > x_max ) position.x = x_max;
		if (position.y > y_max) position.y = y_max;

	}
}