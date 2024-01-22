/* !
@file	Hori_Elevator.h
@author Luke Goh
@date	27/11/2023

This file contains the script for the horizontal elevator
*//*__________________________________________________________________________*/
#pragma once
#include <Object.h>
#include <GameLogic.h>

// Class Hori_Elevator, derived from LogicScript.
// This class is specifically designed to manage a vertical moving platform or elevator within the game. 
// It handles the elevator's movements and direction changes.
class Hori_Elevator : public LogicScript
{
public:
    // Constructor for the Hori_Elevator class.
    // @param name: A string representing the name of the Hori_Elevator instance.
    // It initializes the class and potentially sets up initial states or configurations.
    Hori_Elevator(std::string name);

    // Default destructor for the Hori_Elevator class.
    // Marked as default to use the compiler-generated destructor.
    ~Hori_Elevator() = default;

    // Start method, called when the Hori_Elevator script is first activated.
    // @param obj: A pointer to the Object that this script is attached to.
    // Typically used for initial setup and configuration for the elevator.
    void Start(Object* obj) override;

    // Update method, called on every frame to update the elevator's state.
    // @param obj: A pointer to the Object that this script is attached to.
    // Contains the logic for moving the elevator and changing its direction.
    void Update(Object* obj) override;

    // Shutdown method, called when the Hori_Elevator script is being deactivated or destroyed.
    // @param obj: A pointer to the Object that this script is attached to.
    // Used for cleanup tasks or to reset the state of the elevator.
    void Shutdown(Object* obj) override;

private:
    // A boolean variable to store the moving direction of the elevator.
    // False indicates moving downwards, and true indicates moving upwards.
    // It is used in the Update method to control the elevator's movement.
    bool moving_platform_direction;
};