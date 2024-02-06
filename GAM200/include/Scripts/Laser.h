#pragma once
/* !
@file	Laser.h
@author Luke Goh
@date	27/11/2023

This file contains the script for the horizontal elevator
*//*__________________________________________________________________________*/
#pragma once
#include <Object.h>
#include <GameLogic.h>

// Class Laser, derived from LogicScript.
// This class is specifically designed to manage a vertical moving platform or elevator within the game. 
// It handles the elevator's movements and direction changes.
namespace L_Script {
    class Laser : public LogicScript
    {
    public:
        // Constructor for the Laser class.
        // @param name: A string representing the name of the Laser instance.
        // It initializes the class and potentially sets up initial states or configurations.
        Laser(std::string name);

        // Default destructor for the Laser class.
        // Marked as default to use the compiler-generated destructor.
        ~Laser() = default;

        // Start method, called when the Laser script is first activated.
        // @param obj: A pointer to the Object that this script is attached to.
        // Typically used for initial setup and configuration for the elevator.
        void Start(Object* obj) override;

        // Update method, called on every frame to update the elevator's state.
        // @param obj: A pointer to the Object that this script is attached to.
        // Contains the logic for moving the elevator and changing its direction.
        void Update(Object* obj) override;

        // Shutdown method, called when the Laser script is being deactivated or destroyed.
        // @param obj: A pointer to the Object that this script is attached to.
        // Used for cleanup tasks or to reset the state of the elevator.
        void Shutdown(Object* obj) override;

    private:
        bool active;
    };
}