/* !
@file	Finn.h
@author Luke Goh
@date	27/11/2023

This file contains the script for Finn
*//*__________________________________________________________________________*/
#pragma once
#include <Object.h>
#include <GameLogic.h>

// Class Finn, derived from LogicScript.
// This class is designed to manage the behavior and state of a character or entity named Finn in a game.
// It includes functionalities for initializing, updating, and shutting down the Finn entity.
class Finn : public LogicScript
{
public:
	// Constructor for the Finn class.
	// @param name: A string representing the name of the Finn instance.
	// Initializes the Finn entity and potentially sets up initial states or configurations.
	Finn(std::string);
	~Finn() = default;

	// Start method, called when the Finn script is first activated.
	// @param obj: A pointer to the Object that this script is attached to.
	// Typically used for initial setup and configuration for Finn.
	void Start(Object* obj);
	// Update method, called on every frame to update Finn's state.
	// @param obj: A pointer to the Object that this script is attached to.
	// Contains the logic for Finn's actions, movements, and interactions in the game.
	void Update(Object* obj);

	// Shutdown method, called when the Finn script is being deactivated or destroyed.
	// @param obj: A pointer to the Object that this script is attached to.
	// Used for cleanup tasks or to reset the state of Finn.
	void Shutdown(Object* obj);
};