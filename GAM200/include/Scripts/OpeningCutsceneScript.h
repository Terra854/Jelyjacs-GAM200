/* !
@file	OpeningCutsceneScript.h
@author Tan Yee Ann
@date	13/3/2024

This file contains the script for OpeningCutsceneScript
*//*__________________________________________________________________________*/
#pragma once
#include <Object.h>
#include <GameLogic.h>

// Class OpeningCutsceneScript, derived from LogicScript.
// This class is designed to manage the behavior and state of a character or entity named OpeningCutsceneScript in a game.
// It includes functionalities for initializing, updating, and shutting down the OpeningCutsceneScript entity.
class OpeningCutsceneScript : public LogicScript
{
public:
	// Constructor for the OpeningCutsceneScript class.
	// @param name: A string representing the name of the OpeningCutsceneScript instance.
	// Initializes the OpeningCutsceneScript entity and potentially sets up initial states or configurations.
	OpeningCutsceneScript(std::string);
	~OpeningCutsceneScript() = default;

	// Start method, called when the OpeningCutsceneScript script is first activated.
	// @param obj: A pointer to the Object that this script is attached to.
	// Typically used for initial setup and configuration for OpeningCutsceneScript.
	void Start(Object* obj);
	// Update method, called on every frame to update OpeningCutsceneScript's state.
	// @param obj: A pointer to the Object that this script is attached to.
	// Contains the logic for OpeningCutsceneScript's actions, movements, and interactions in the game.
	void Update(Object* obj);

	// Shutdown method, called when the OpeningCutsceneScript script is being deactivated or destroyed.
	// @param obj: A pointer to the Object that this script is attached to.
	// Used for cleanup tasks or to reset the state of OpeningCutsceneScript.
	void Shutdown(Object* obj);
};