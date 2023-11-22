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
	virtual std::string SystemName() { return "Camera"; }

	void SetToPlayer();

	void toggleFreeCam() { free_cam = !free_cam; }
	bool isFreeCamEnabled() { return free_cam; }

private:
	bool free_cam = false;

};
extern Camera* camera2D;
#endif // !CAMERA_H