#pragma once
/* !
@file Box.h
@author Luke Goh
@date	7/2/2024

This file contains the script for the box that Finn can move
*//*__________________________________________________________________________*/
#pragma once
#include <Object.h>
#include <GameLogic.h>

// Class Box, derived from the LogicScript class.
// This class defines the behavior of a box in a game, only allowing Finn to move it.
class Box : public LogicScript
{
public:
    // Constructor for the Box class.
    // @param name: A string representing the name of the Box instance.
    Box(std::string name);

    // Default destructor for the Box class.
    // It is marked as default to use the compiler-generated destructor.
    ~Box() = default;

    // Start method, called when the Box script is first activated.
    // @param obj: A pointer to the Object that this script is attached to.
    // This method typically performs initial setup and configuration.
    void Start(Object* obj);

    // Update method, called on every frame of the game to update the state of the box.
    // @param obj: A pointer to the Object that this script is attached to.
    // This method typically performs the main behavior of the box.
    void Update(Object* obj);

    // Shutdown method, called when the Box script is being deactivated or destroyed.
    // @param obj: A pointer to the Object that this script is attached to.
    // This method typically performs cleanup tasks.
    void Shutdown(Object* obj);
};
