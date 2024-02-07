/* !
@file	PressurePlate.h
@author Luke Goh
@date	7/2/2024

This file contains the script for PPlate
*//*__________________________________________________________________________*/
#pragma once
#include <Object.h>
#include <GameLogic.h>

// Class PPlate, derived from LogicScript.
// This class is intended to encapsulate the behavior and logic of a pressure plate mechanism in a game.
// It provides a framework for managing the pressure plate's initialization, regular updates, and shutdown process.
namespace PressurePlate {
    class PPlate : public LogicScript
    {
    public:
        // Constructor for the PPlate class.
        // @param name: A string representing the name of the PPlate instance.
        // This constructor initializes the pressure plate, setting up any necessary states or configurations.
        PPlate(std::string name);

        // Default destructor for the PPlate class.
        // Marked as default to use the compiler-generated destructor.
        ~PPlate() = default;

        // Start method, called when the PPlate script is first activated.
        // @param obj: A pointer to the Object that this script is attached to.
        // Typically used for initial setup and configuration of the pressure plate.
        void Start(Object* obj);

        // Update method, called on every frame to update the pressure plate's state.
        // @param obj: A pointer to the Object that this script is attached to.
        // Contains the logic for the pressure plate's movement, interactions, and other behaviors.
        void Update(Object* obj);

        // Shutdown method, called when the PPlate script is being deactivated or destroyed.
        // @param obj: A pointer to the Object that this script is attached to.
        // Used for cleanup tasks or to reset the state of the pressure plate.
        void Shutdown(Object* obj);
    };
}