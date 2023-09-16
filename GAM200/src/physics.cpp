#include "physics.h"

/*
* This function is meant to update the gravity of a game object.
* In the future, this will be changed to take in a game object struct
* once we implement that in the game state update function.
*/
void gravityUpdate(int* gameobjectYVelocity)
{
	*gameobjectYVelocity = gravity * frameTime + *gameobjectYVelocity;
}