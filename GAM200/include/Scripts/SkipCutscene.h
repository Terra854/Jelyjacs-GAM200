#pragma once
/* !
@file	SkipCutscene.h
@author Tan Yee Ann (t.yeeann@digipen.edu)
@date	26/2/2023

This file contains the script for the SkipCutscenes
*//*__________________________________________________________________________*/
#include <Object.h>
#include <GameLogic.h>

// Class SkipCutscene, derived from the LogicScript class.
// This class defines the behavior of the clickable buttons, of which, the rest of the in-game buttons derive from.
class SkipCutscene : public LogicScript
{
public:
    // Constructor for the SkipCutscene class.
    // @param name: A string representing the name of the SkipCutscene instance.
    SkipCutscene(std::string name);

    // Default destructor for the SkipCutscene class.
    // It is marked as default to use the compiler-generated destructor.
    ~SkipCutscene() = default;

    // Start method, called when the SkipCutscene script is first activated.
    // @param obj: A pointer to the Object that this script is attached to.
    // This method typically performs initial setup and configuration.
    void Start(Object* obj);

    // Update method, called on every frame of the game to update the state of the portal.
    // @param obj: A pointer to the Object that this script is attached to.
    // This method contains the main logic for the portal's behavior, such as checking for an object's proximity and teleporting it.
    void Update(Object* obj);

    // Shutdown method, called when the SkipCutscene script is being deactivated or destroyed.
    // @param obj: A pointer to the Object that this script is attached to.
    // This method typically performs cleanup tasks.
    void Shutdown(Object* obj);
};
