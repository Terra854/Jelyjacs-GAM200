/* !
@file	Endpoint.h
@author Luke Goh
@date	27/11/2023

This file contains the script for the Endpoint
*//*__________________________________________________________________________*/
#pragma once
#include <Object.h>	
#include <GameLogic.h>

// Class Endpoint, derived from LogicScript.
// This class is designed to manage the behavior and state of an endpoint object in a game.
// It includes functionalities for initializing, updating, and shutting down the endpoint object.
class Endpoint : public LogicScript
{
public: 
	// Constructor for the Endpoint class.
	// @param name: A string representing the name of the Endpoint instance.
	// Initializes the Endpoint object and potentially sets up initial states or configurations.
	Endpoint(std::string);
	~Endpoint() = default;

	// Start method, called when the Endpoint script is first activated.
	// @param obj: A pointer to the Object that this script is attached to.
	// Typically used for initial setup and configuration for the endpoint object.
	void Start(Object* obj);

	// Update method, called on every frame to update the endpoint object's state.
	// @param obj: A pointer to the Object that this script is attached to.
	// Contains the logic for the endpoint object's behavior and interactions during the game.
	void Update(Object* obj);

	// Shutdown method, called when the Endpoint script is being deactivated or destroyed.
	// @param obj: A pointer to the Object that this script is attached to.
	// Used for cleanup tasks or to reset the state of the endpoint object.
	void Shutdown(Object* obj);
};