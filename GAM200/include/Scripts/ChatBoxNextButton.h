#pragma once
/* !
@author Yeo Jia Ming
@date	23/3/2024

This file contains the script for the next dialogue
*//*__________________________________________________________________________*/
#include <Object.h>
#include <GameLogic.h>
#include <Core_Engine.h>

// Class ButtonBase, derived from the LogicScript class.
// This class defines the behavior of the clickable buttons, of which, the rest of the in-game buttons derive from.
class ChatBoxNextButton : public LogicScript
{
public:
    // Constructor for the ButtonBase class.
    // @param name: A string representing the name of the ButtonBase instance.
    ChatBoxNextButton(std::string name);

    // Default destructor for the ButtonBase class.
    // It is marked as default to use the compiler-generated destructor.
    ~ChatBoxNextButton() = default;

    // Start method, called when the ButtonBase script is first activated.
    // @param obj: A pointer to the Object that this script is attached to.
    // This method typically performs initial setup and configuration.
    void Start(Object* obj);

    // Update method, called on every frame of the game to update the state of the portal.
    // @param obj: A pointer to the Object that this script is attached to.
    // This method contains the main logic for the portal's behavior, such as checking for an object's proximity and teleporting it.
    void Update(Object* obj);

    // Shutdown method, called when the ButtonBase script is being deactivated or destroyed.
    // @param obj: A pointer to the Object that this script is attached to.
    // This method typically performs cleanup tasks.
    void Shutdown(Object* obj);

};
