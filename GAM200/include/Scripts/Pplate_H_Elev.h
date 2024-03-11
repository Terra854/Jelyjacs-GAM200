#pragma once
/* !
@file	Pplate_H_Elev.h
@author Luke Goh
@date	7/2/2024

This file contains the script for Pplate_H_Elev
*//*__________________________________________________________________________*/
#pragma once
#include <Object.h>
#include <GameLogic.h>

// Class Pplate_H_Elev, derived from LogicScript.
// This class is intended to encapsulate the behavior and logic of a piston mechanism in a game.
// It provides a framework for managing the piston's initialization, regular updates, and shutdown process.
namespace PHE_Script{
    class Pplate_H_Elev : public LogicScript
    {
    public:
        // Constructor for the Pplate_H_Elev class.
        // @param name: A string representing the name of the Pplate_H_Elev instance.
        // This constructor initializes the piston and horizontal elevator, setting up any necessary states or configurations.
        Pplate_H_Elev(std::string name);

        // Default destructor for the Pplate_H_Elev class.
        // Marked as default to use the compiler-generated destructor.
        ~Pplate_H_Elev() = default;

        // Start method, called when the Pplate_H_Elev script is first activated.
        // @param obj: A pointer to the Object that this script is attached to.
        // Typically used for initial setup and configuration of the piston and horizontal elevator.
        void Start(Object* obj);

        // Update method, called on every frame to update the piston's state.
        // @param obj: A pointer to the Object that this script is attached to.
        // Contains the logic for detecting piston collision and reacting by moving the horizontal platform.
        void Update(Object* obj);

        // Shutdown method, called when the Pplate_H_Elev script is being deactivated or destroyed.
        // @param obj: A pointer to the Object that this script is attached to.
        // Used for cleanup tasks or to reset the state of the piston.
        void Shutdown(Object* obj);

    private:
        bool moving_platform_direction; // A boolean variable to store the moving direction of the horizontal elevator.
        bool activated; // A boolean variable to store the activation state of the piston.
        Object* H_Elev; // Pointer to the horizontal elevator object.
    };
}