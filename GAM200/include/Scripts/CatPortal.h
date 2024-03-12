/* !
@file CatPortal.h
@author Luke Goh
@date	27/11/2023

This file contains the script for the portal for the cat character to teleport
*//*__________________________________________________________________________*/
#pragma once
#include <Object.h>
#include <GameLogic.h>

// Class CatPortal, derived from the LogicScript class.
// This class defines the behavior of a portal in a game, allowing objects (like a player) to teleport between locations.
namespace CatPortal_Script {
    class CatPortal : public LogicScript
    {
    public:
        // Constructor for the CatPortal class.
        // @param name: A string representing the name of the CatPortal instance.
        CatPortal(std::string name);

        // Default destructor for the CatPortal class.
        // It is marked as default to use the compiler-generated destructor.
        ~CatPortal() = default;

        // Start method, called when the CatPortal script is first activated.
        // @param obj: A pointer to the Object that this script is attached to.
        // This method typically performs initial setup and configuration.
        void Start(Object* obj);

        // Update method, called on every frame of the game to update the state of the portal.
        // @param obj: A pointer to the Object that this script is attached to.
        // This method contains the main logic for the portal's behavior, such as checking for an object's proximity and teleporting it.
        void Update(Object* obj);

        // Shutdown method, called when the CatPortal script is being deactivated or destroyed.
        // @param obj: A pointer to the Object that this script is attached to.
        // This method typically performs cleanup tasks.
        void Shutdown(Object* obj);

        // Static member variable to track whether an object (like a player) has just teleported.
        // This is to prevent continuous teleportation in and out of the portal.
        static bool justTeleported;
    };
} // namespace CatPortal_Script