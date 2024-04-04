/* !
@file	EndingCutscene.h
@author Tan Yee Ann
@date	13/3/2024

This file contains the script for EndingCutscene
*//*__________________________________________________________________________*/
#pragma once
#include <Object.h>
#include <GameLogic.h>

// Class EndingCutscene, derived from LogicScript.
// This class is designed to manage the behavior and state of a character or entity named EndingCutscene in a game.
// It includes functionalities for initializing, updating, and shutting down the EndingCutscene entity.
class EndingCutscene : public LogicScript
{
public:
	// Constructor for the EndingCutscene class.
	// @param name: A string representing the name of the EndingCutscene instance.
	// Initializes the EndingCutscene entity and potentially sets up initial states or configurations.
	EndingCutscene(std::string);
	~EndingCutscene() = default;

	// Start method, called when the EndingCutscene script is first activated.
	// @param obj: A pointer to the Object that this script is attached to.
	// Typically used for initial setup and configuration for EndingCutscene.
	void Start(Object* obj);
	// Update method, called on every frame to update EndingCutscene's state.
	// @param obj: A pointer to the Object that this script is attached to.
	// Contains the logic for EndingCutscene's actions, movements, and interactions in the game.
	void Update(Object* obj);

	// Shutdown method, called when the EndingCutscene script is being deactivated or destroyed.
	// @param obj: A pointer to the Object that this script is attached to.
	// Used for cleanup tasks or to reset the state of EndingCutscene.
	void Shutdown(Object* obj);
};