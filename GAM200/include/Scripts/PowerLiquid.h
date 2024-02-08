#pragma once
/* !
@file PowerLiquid.h
@author Luke Goh
@date	7/2/2024

This file contains the script for the cat power liquid for player to collect
*//*__________________________________________________________________________*/
#pragma once
#include <Object.h>
#include <GameLogic.h>

// Class P_liquid, derived from the LogicScript class.
// This class defines the behavior of a cat power liquid in a game, only allowing Finn to move it.
namespace PL_Script {
    class P_liquid : public LogicScript
    {
    public:
        // Constructor for the P_liquid class.
        // @param name: A string representing the name of the P_liquid instance.
        P_liquid(std::string name);

        // Default destructor for the P_liquid class.
        // It is marked as default to use the compiler-generated destructor.
        ~P_liquid() = default;

        // Start method, called when the P_liquid script is first activated.
        // @param obj: A pointer to the Object that this script is attached to.
        // This method typically performs initial setup and configuration.
        void Start(Object* obj);

        // Update method, called on every frame of the game to update the state of the Cat Liquid.
        // @param obj: A pointer to the Object that this script is attached to.
        // This method typically performs the main behavior of the Cat Liquid.
        void Update(Object* obj);

        // Shutdown method, called when the P_liquid script is being deactivated or destroyed.
        // @param obj: A pointer to the Object that this script is attached to.
        // This method typically performs cleanup tasks.
        void Shutdown(Object* obj);
    };
} // namespace PL_Script
