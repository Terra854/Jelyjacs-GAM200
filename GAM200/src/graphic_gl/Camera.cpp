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
	static float accum_time = 0.0f;
	static int frame_dt_count = 0;


	if (!isFreeCamEnabled()) {
		if (camera_shake) {
			// get a random number between -1 to 1
			float x = (rand() % 1000) / 1000.0f;
			float y = (rand() % 1000) / 1000.0f;
			random_num = { x * 2.0f - 1.0f, y * 2.0f - 1.0f };
			random_num.x *= random_shift.x;
			random_num.y *= random_shift.y;
			
			position += random_num;
			time_count += engine->GetDt();
			std::cout << "time_count" << time_count << std::endl;
			if (time_count >= time_shift) {
				time_count = 0.0f;
				camera_shake = false;
				random_shift = { 0.0f, 0.0f };
				camera_follow = true;
			}

		}
		else if (camera_shift) {
			position -= camera_speed * engine->Get_Fixed_DT();
			time_count += engine->Get_Fixed_DT();
			if (time_count >= time_shift) {
				time_count = 0.0f;
				camera_follow = true;
				camera_shift = false;
			}
		}
		else  SetToPlayer();


	}
	world_to_ndc = Mat3Scale(scale.x, scale.y) * Mat3Translate(position.x, position.y);

}

/*  _________________________________________________________________________ */
/*
* Set the camera scale
* @param scale_input The scale of the camera
*
*/
void Camera::SetCameraScale(Vec2 scale_input)
{
	scale.x = scale_input.x / window->width_init;
	scale.y = scale_input.y / window->height_init;
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

		position.x = trans->Position.x;

		position.y = trans->Position.y;

		// make sure the camera is not out of bounds
		Vec2 botleft = engine->Get_Start_Coords();
		Vec2 topright = engine->Get_End_Coords();
		if (position.x < botleft.x + window->width * 0.5 / scale.x) {
			position.x = botleft.x + window->width * 0.5 / scale.x;
		}
		if (position.y < botleft.y + window->height * 0.5 / scale.y) {
			position.y = botleft.y + window->height * 0.5 / scale.y;
		}
		if (position.x > topright.x - window->width * 0.5 / scale.x) {
			position.x = topright.x - window->width * 0.5 / scale.x;
		}
		if (position.y > topright.y - window->height * 0.5 / scale.y) {
			position.y = topright.y - window->height * 0.5 / scale.y;
		}
	}
	else {
		position = { 0.0f, 0.0f };
	}

	Vec2 level_size = (engine->Get_End_Coords() - engine->Get_Start_Coords()) * scale.x;
	if (level_size.x < window->width) {
		position.x = engine->Get_Start_Coords().x + (level_size.x / 2.0f);
	}
	if (level_size.y < window->height) {
		position.y = engine->Get_Start_Coords().y + (level_size.y / 2.0f);
	}

	position.x = -position.x * 2.0f / window->width_init;
	position.y = -position.y * 2.0f / window->height_init;
}

/*  _________________________________________________________________________ */
/*
* Translate the camera from start to end in time
* NDC standers for start and end
*/
void Camera::TranslateCamera(Vec2 start, Vec2 end, float time)
{
	if (scale.x == 1.0f && scale.y == 1.0f)return;
	camera_shift = true;
	Vec2 pos1, pos2;
	pos1.x = start.x;
	pos1.y = start.y;
	pos2.x = end.x;
	pos2.y = end.y;

	// make sure the camera is not out of bounds
	Vec2 botleft = engine->Get_Start_Coords();
	Vec2 topright = engine->Get_End_Coords();
	if (pos1.x < botleft.x + window->width * 0.5 / scale.x) {
		pos1.x = botleft.x + window->width * 0.5 / scale.x;
	}
	if (pos1.y < botleft.y + window->height * 0.5 / scale.y) {
		pos1.y = botleft.y + window->height * 0.5 / scale.y;
	}
	if (pos1.x > topright.x - window->width * 0.5 / scale.x) {
		pos1.x = topright.x - window->width * 0.5 / scale.x;
	}
	if (pos1.y > topright.y - window->height * 0.5 / scale.y) {
		pos1.y = topright.y - window->height * 0.5 / scale.y;
	}

	if (pos2.x < botleft.x + window->width * 0.5 / scale.x) {
		pos2.x = botleft.x + window->width * 0.5 / scale.x;
	}
	if (pos2.y < botleft.y + window->height * 0.5 / scale.y) {
		pos2.y = botleft.y + window->height * 0.5 / scale.y;
	}
	if (pos2.x > topright.x - window->width * 0.5 / scale.x) {
		pos2.x = topright.x - window->width * 0.5 / scale.x;
	}
	if (pos2.y > topright.y - window->height * 0.5 / scale.y) {
		pos2.y = topright.y - window->height * 0.5 / scale.y;
	}

	// convert to NDC
	pos1.x = -pos1.x * 2.0f / window->width_init;
	pos1.y = -pos1.y * 2.0f / window->height_init;
	pos2.x = -pos2.x * 2.0f / window->width_init;
	pos2.y = -pos2.y * 2.0f / window->height_init;

	camera_speed = (pos1 - pos2) / time;
	position = pos1;
	time_count = 0.0f;
	time_shift = time;
	camera_follow = false;
}

/*  _________________________________________________________________________ */
/*
* Shake the camera with a range for a time
* NDC standers for range
*/
void Camera::ShakeCamera(Vec2 range, float time)
{
	random_shift = range;
	camera_shake = true;
	time_count = 0.0f;
	time_shift = time;
	camera_follow = false;
}
