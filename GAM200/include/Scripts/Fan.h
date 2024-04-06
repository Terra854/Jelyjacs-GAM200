/* !
@file	Fan.h
@author Tan Yee Ann
@date	6/4/2024

This file contains the script for Fan
*//*__________________________________________________________________________*/
#pragma once
#include <Object.h>
#include <GameLogic.h>
#include <Audio.h>

// Class Fan, derived from LogicScript.
// This class is designed to manage the behavior and state of a character or entity named Fan in a game.
// It includes functionalities for initializing, updating, and shutting down the Fan entity.
class Fan : public LogicScript
{
public:
	// Constructor for the Fan class.
	// @param name: A string representing the name of the Fan instance.
	// Initializes the Fan entity and potentially sets up initial states or configurations.
	Fan(std::string);
	~Fan() = default;

	// Start method, called when the Fan script is first activated.
	// @param obj: A pointer to the Object that this script is attached to.
	// Typically used for initial setup and configuration for Fan.
	void Start(Object* obj);
	// Update method, called on every frame to update Fan's state.
	// @param obj: A pointer to the Object that this script is attached to.
	// Contains the logic for Fan's actions, movements, and interactions in the game.
	void Update(Object* obj);

	// Shutdown method, called when the Fan script is being deactivated or destroyed.
	// @param obj: A pointer to the Object that this script is attached to.
	// Used for cleanup tasks or to reset the state of Fan.
	void Shutdown(Object* obj);
};