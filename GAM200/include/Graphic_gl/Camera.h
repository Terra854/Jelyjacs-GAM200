/* !
@file    Camera.h
@author  Guo Chen (g.chen@digipen.edu)
@date    02/11/2023

Header file for Camera.cpp
*//*__________________________________________________________________________*/
#include <Mat3.h>
#include <Vec2.h>
#include <interface_System.h>
#include <components/Transform.h>
#include <components/Physics.h>
#include <components/Body.h>
#include <components/PlayerControllable.h>
#include <Factory.h>
#include <GLWindow.h>
#include <GameLogic.h>
#include <glapp.h>

#ifndef CAMERA_H
#define CAMERA_H

class Camera : public ISystems
{
	public:
		Camera();
		~Camera() {}
	
		Vec2 position;
		Vec2 scale;
		Mat3 world_to_ndc;

	virtual void Initialize();
	virtual void Update();

	// Returns name of the component
	virtual std::string SystemName() { return "Camera"; }

	void SetCameraScale(Vec2 scale_input);
	void SetCameraPosition(Vec2 position_input);
	void SetToPlayer();

	void TranslateCamera(Vec2 start, Vec2 end, float time);

	// Toggles the free camera
	void toggleFreeCam() { free_cam = !free_cam; }

	// Check if the free camera is enabled
	bool isFreeCamEnabled() { return free_cam; }

	// Set free cam status
	void setFreeCamEnabled(bool b) { free_cam = b; }

	void updatelimit(float x_min_, float y_min_, float x_max_, float y_max_) {
		this->x_min = x_min_;
		this->y_min = y_min_;
		this->x_max = x_max_;
		this->y_max = y_max_;
	}

	float game_to_camera(float in){
		return in * 2.0f / window->width_init;
	}


private:
	bool free_cam = false;
	bool camera_follow = true;
	Vec2 camera_speed = {0.0f,0.0f};
	float time_count = 0.0f;
	float time_shift = 0.0f;
	float x_min = game_to_camera(-672.0f);
	float y_min = game_to_camera(-352.0f);
	float x_max = game_to_camera(-672.0f + 2176.0f);
	float y_max = game_to_camera(-352.0f + 1344.0f);


};
extern Camera* camera2D;
#endif // !CAMERA_H