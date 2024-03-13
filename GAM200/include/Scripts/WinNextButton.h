#pragma once
/* !
@file	.h
@author 
@date	26/2/2023

This file contains the script for the Quit Button
*//*__________________________________________________________________________*/
#include <Object.h>
#include <GameLogic.h>

class WinNextButton : public LogicScript
{
public:
    WinNextButton(std::string name);

    ~WinNextButton() = default;

    void Start(Object* obj);

    void Update(Object* obj);

    void Shutdown(Object* obj);
};
