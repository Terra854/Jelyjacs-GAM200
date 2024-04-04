/* !
@file	OpeningCutscene.h
@author Tan Yee Ann
@date	13/3/2024

This file contains the script for OpeningCutscene
*//*__________________________________________________________________________*/
#pragma once
#include <Object.h>
#include <GameLogic.h>
#include <Audio.h>

extern FMOD::ChannelGroup* rain_sfx, * people_sfx;

// Class OpeningCutscene, derived from LogicScript.
// This class is designed to manage the behavior and state of a character or entity named OpeningCutscene in a game.
// It includes functionalities for initializing, updating, and shutting down the OpeningCutscene entity.
class OpeningCutscene : public LogicScript
{
public:
	// Constructor for the OpeningCutscene class.
	// @param name: A string representing the name of the OpeningCutscene instance.
	// Initializes the OpeningCutscene entity and potentially sets up initial states or configurations.
	OpeningCutscene(std::string);
	~OpeningCutscene() = default;

	// Start method, called when the OpeningCutscene script is first activated.
	// @param obj: A pointer to the Object that this script is attached to.
	// Typically used for initial setup and configuration for OpeningCutscene.
	void Start(Object* obj);
	// Update method, called on every frame to update OpeningCutscene's state.
	// @param obj: A pointer to the Object that this script is attached to.
	// Contains the logic for OpeningCutscene's actions, movements, and interactions in the game.
	void Update(Object* obj);

	// Shutdown method, called when the OpeningCutscene script is being deactivated or destroyed.
	// @param obj: A pointer to the Object that this script is attached to.
	// Used for cleanup tasks or to reset the state of OpeningCutscene.
	void Shutdown(Object* obj);
};