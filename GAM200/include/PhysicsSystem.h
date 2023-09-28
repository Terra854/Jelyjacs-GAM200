#pragma once
/* !
@file	PhysicsSystem.h
@author	Tan Yee Ann
@date	28/9/2023

This file contains the declaration for the Physics system class
*//*__________________________________________________________________________*/
#include <Debug.h>

#include <Interface_System.h>

const float gravity = -3000.0f;

class PhysicsSystem : public ISystems {
public:
	PhysicsSystem();
	~PhysicsSystem() {};
	virtual void Initialize();
	virtual void Update(float time);
	virtual void MessageRelay(Message_Handler* msg);
	virtual std::string SystemName() { return "Physics"; }
};