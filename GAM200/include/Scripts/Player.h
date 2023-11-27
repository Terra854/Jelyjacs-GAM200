/* !
@file	Player.h
@author Luke Goh
@date	27/11/2023

This file contains the script for Player
*//*__________________________________________________________________________*/
#pragma once
#include <Object.h>
#include <GameLogic.h>

// Class Player, derived from LogicScript.
// This class is specifically tailored to handle the logic and behavior of a player character within a game.
// It includes methods to manage the player's initialization, regular updates, and shutdown process.
class Player : public LogicScript
{
public:
    // Constructor for the Player class.
    // @param name: A string representing the name of the Player instance.
    // This constructor initializes the player and potentially sets up initial states or configurations.
    Player(std::string name);

    // Default destructor for the Player class.
    // Marked as default to use the compiler-generated destructor.
    ~Player() = default;

    // Start method, overridden from LogicScript, called when the Player script is first activated.
    // @param obj: A pointer to the Object that this script is attached to.
    // Typically used for initial setup and configuration for the player character.
    void Start(Object* obj) override;

    // Update method, overridden from LogicScript, called on every frame to update the player's state.
    // @param obj: A pointer to the Object that this script is attached to.
    // Contains the logic for handling player actions, movements, and interactions in the game.
    void Update(Object* obj) override;

    // Shutdown method, overridden from LogicScript, called when the Player script is being deactivated or destroyed.
    // @param obj: A pointer to the Object that this script is attached to.
    // Used for cleanup tasks or to reset the state of the player character.
    void Shutdown(Object* obj) override;
};
