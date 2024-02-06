#pragma once
/* !
@file	Piston_H_Elev.h
@author Luke Goh
@date	27/11/2023

This file contains the script for Piston_H_Elev
*//*__________________________________________________________________________*/
#pragma once
#include <Object.h>
#include <GameLogic.h>

// Class Piston_H_Elev, derived from LogicScript.
// This class is intended to encapsulate the behavior and logic of a piston mechanism in a game.
// It provides a framework for managing the piston's initialization, regular updates, and shutdown process.
namespace PHE_Script{
    class Piston_H_Elev : public LogicScript
    {
    public:
        // Constructor for the Piston_H_Elev class.
        // @param name: A string representing the name of the Piston_H_Elev instance.
        // This constructor initializes the piston, setting up any necessary states or configurations.
        Piston_H_Elev(std::string name);

        // Default destructor for the Piston_H_Elev class.
        // Marked as default to use the compiler-generated destructor.
        ~Piston_H_Elev() = default;

        // Start method, called when the Piston_H_Elev script is first activated.
        // @param obj: A pointer to the Object that this script is attached to.
        // Typically used for initial setup and configuration of the piston.
        void Start(Object* obj);

        // Update method, called on every frame to update the piston's state.
        // @param obj: A pointer to the Object that this script is attached to.
        // Contains the logic for the piston's movement, interactions, and other behaviors.
        void Update(Object* obj);

        // Shutdown method, called when the Piston_H_Elev script is being deactivated or destroyed.
        // @param obj: A pointer to the Object that this script is attached to.
        // Used for cleanup tasks or to reset the state of the piston.
        void Shutdown(Object* obj);

    private:
        // A boolean variable to store the moving direction of the piston.
        // False indicates moving downwards, and true indicates moving upwards.
        // It is used in the Update method to control the piston's movement.
        bool moving_platform_direction;
        bool activated;
        Object* H_Elev;
    };
}