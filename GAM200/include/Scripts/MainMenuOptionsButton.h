#pragma once
/* !
@file	MainMenuOptionsButton.h
@author
@date	26/2/2023

This file contains the script for the Quit Button
*//*__________________________________________________________________________*/
#include <Object.h>
#include <GameLogic.h>

class MainMenuOptionsButton : public LogicScript
{
public:
    MainMenuOptionsButton(std::string name);

    ~MainMenuOptionsButton() = default;

    void Start(Object* obj);

    void Update(Object* obj);

    void Shutdown(Object* obj);
};
