#pragma once
#include <Debug.h>

#include <Interface_System.h>

//float frameTime = 1/60.0f;
const float gravity = -20.0f;

/*
* This function is meant to update the gravity of a game object.
* In the future, this will be changed to take in a game object struct
* once we implement that in the game state update function.
*/
void gravityUpdate(int* gameobjectYVelocity);


class Physics : public ISystems {
public:
	Physics();
	virtual void Initialize();
	virtual void Update(float time);
	virtual std::string GetSystemName() { return "Physics"; }
};