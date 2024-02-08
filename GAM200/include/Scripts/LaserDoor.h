#pragma once
/* !
@file	LaserDoor.h
@author Tan Yee Ann
@date	8/2/2023

This file contains the script for the laser "doors"
*//*__________________________________________________________________________*/
#include <Object.h>
#include <GameLogic.h>

// Class LaserDoor, derived from the LogicScript class.
// This class defines the behavior of a portal in a game, allowing objects (like a player) to teleport between locations.
class LaserDoor : public LogicScript
{
public:
    // Constructor for the LaserDoor class.
    // @param name: A string representing the name of the LaserDoor instance.
    LaserDoor(std::string name);

    // Default destructor for the LaserDoor class.
    // It is marked as default to use the compiler-generated destructor.
    ~LaserDoor() = default;

    // Start method, called when the LaserDoor script is first activated.
    // @param obj: A pointer to the Object that this script is attached to.
    // This method typically performs initial setup and configuration.
    void Start(Object* obj);

    // Update method, called on every frame of the game to update the state of the portal.
    // @param obj: A pointer to the Object that this script is attached to.
    // This method contains the main logic for the portal's behavior, such as checking for an object's proximity and teleporting it.
    void Update(Object* obj);

    // Shutdown method, called when the LaserDoor script is being deactivated or destroyed.
    // @param obj: A pointer to the Object that this script is attached to.
    // This method typically performs cleanup tasks.
    void Shutdown(Object* obj);
};