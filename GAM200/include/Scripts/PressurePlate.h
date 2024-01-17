/* !
@file	PPlate.h
@author Luke Goh
@date	27/11/2023

This file contains the script for PPlate
*//*__________________________________________________________________________*/
#pragma once
#include <Object.h>
#include <GameLogic.h>

// Class PPlate, derived from LogicScript.
// This class is intended to encapsulate the behavior and logic of a piston mechanism in a game.
// It provides a framework for managing the piston's initialization, regular updates, and shutdown process.
class PPlate : public LogicScript
{
public:
    // Constructor for the PPlate class.
    // @param name: A string representing the name of the PPlate instance.
    // This constructor initializes the piston, setting up any necessary states or configurations.
    PPlate(std::string name);

    // Default destructor for the PPlate class.
    // Marked as default to use the compiler-generated destructor.
    ~PPlate() = default;

    // Start method, called when the PPlate script is first activated.
    // @param obj: A pointer to the Object that this script is attached to.
    // Typically used for initial setup and configuration of the piston.
    void Start(Object* obj);

    // Update method, called on every frame to update the piston's state.
    // @param obj: A pointer to the Object that this script is attached to.
    // Contains the logic for the piston's movement, interactions, and other behaviors.
    void Update(Object* obj);

    // Shutdown method, called when the PPlate script is being deactivated or destroyed.
    // @param obj: A pointer to the Object that this script is attached to.
    // Used for cleanup tasks or to reset the state of the piston.
    void Shutdown(Object* obj);
};