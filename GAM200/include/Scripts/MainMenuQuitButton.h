#pragma once
/* !
@file	MainMenuQuitButton.h
@author
@date	26/2/2023

This file contains the script for the Quit Button
*//*__________________________________________________________________________*/
#include <Object.h>
#include <GameLogic.h>

class MainMenuQuitButton : public LogicScript
{
public:
    MainMenuQuitButton(std::string name);

    ~MainMenuQuitButton() = default;

    void Start(Object* obj);

    void Update(Object* obj);

    void Shutdown(Object* obj);
};
