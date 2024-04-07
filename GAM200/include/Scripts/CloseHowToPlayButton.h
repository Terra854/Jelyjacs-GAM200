#pragma once
/* !
@author Yeo Jia Ming
@date	10/3/2024

This file contains the script for the next dialogue
*//*__________________________________________________________________________*/
#include <Object.h>
#include <GameLogic.h>

class CloseHowToPlayMenuButton : public LogicScript
{
public:
    CloseHowToPlayMenuButton(std::string name);

    ~CloseHowToPlayMenuButton() = default;

    void Start(Object* obj);

    void Update(Object* obj);

    void Shutdown(Object* obj);
};
