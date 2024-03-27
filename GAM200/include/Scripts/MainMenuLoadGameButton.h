#pragma once
/* !
@file	MainMenuLoadGameButton.h
@author
@date	26/2/2023

This file contains the script for the Quit Button
*//*__________________________________________________________________________*/
#include <Object.h>
#include <GameLogic.h>

class MainMenuLoadGameButton : public LogicScript
{
public:
    MainMenuLoadGameButton(std::string name);

    ~MainMenuLoadGameButton() = default;

    void Start(Object* obj);

    void Update(Object* obj);

    void Shutdown(Object* obj);
};
