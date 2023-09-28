#pragma once
/* !
@file
@author	t.yeeann@digipen.edu
@date	28/9/2023


*//*__________________________________________________________________________*/
#include <Debug.h>

#include <Interface_System.h>

//float frameTime = 1/60.0f;
const float gravity = -3000.0f;

/*
* This function is meant to update the gravity of a game object.
* In the future, this will be changed to take in a game object struct
* once we implement that in the game state update function.
*/
//void gravityUpdate(int* objYVelocity);


class PhysicsSystem : public ISystems {
public:
	PhysicsSystem();
	~PhysicsSystem() {};
	virtual void Initialize();
	virtual void Update(float time);
	virtual void MessageRelay(Message_Handler* msg);
	virtual std::string SystemName() { return "Physics"; }
};