#pragma once
/* !
@author Yeo Jia Ming
@date	10/3/2024

This file contains the script for restart button in pause menu
*//*__________________________________________________________________________*/
#include <Object.h>
#include <GameLogic.h>

// Class RestartButton, derived from the LogicScript class.
// This class defines the behavior of the clickable buttons, of which, the rest of the in-game buttons derive from.
class RestartButton : public LogicScript
{
public:
    // Constructor for the RestartButton class.
    // @param name: A string representing the name of the RestartButton instance.
    RestartButton(std::string name);

    // Default destructor for the RestartButton class.
    // It is marked as default to use the compiler-generated destructor.
    ~RestartButton() = default;

    // Start method, called when the RestartButton script is first activated.
    // @param obj: A pointer to the Object that this script is attached to.
    // This method typically performs initial setup and configuration.
    void Start(Object* obj);

    // Update method, called on every frame of the game to update the state of the portal.
    // @param obj: A pointer to the Object that this script is attached to.
    // This method contains the main logic for the portal's behavior, such as checking for an object's proximity and teleporting it.
    void Update(Object* obj);

    // Shutdown method, called when the RestartButton script is being deactivated or destroyed.
    // @param obj: A pointer to the Object that this script is attached to.
    // This method typically performs cleanup tasks.
    void Shutdown(Object* obj);
};
