#include <Mat3.h>
#include <Vec2.h>
#include <interface_System.h>
#include <components/Transform.h>
#include <components/Physics.h>
#include <components/Body.h>
#include <components/PlayerControllable.h>
#include <Factory.h>
#include <GLWindow.h>

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

};
extern Camera* camera2D;
#endif // !CAMERA_H