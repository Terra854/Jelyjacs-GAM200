#pragma once
/* !
@file	.h
@author 
@date	26/2/2023

This file contains the script for the Quit Button
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
