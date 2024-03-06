#pragma once
/* !
@file	QuitButton.h
@author 
@date	26/2/2023

This file contains the script for the Quit Button
*//*__________________________________________________________________________*/
#include <Object.h>
#include <GameLogic.h>

class QuitButton : public LogicScript
{
public:
    QuitButton(std::string name);

    ~QuitButton() = default;

    void Start(Object* obj);

    void Update(Object* obj);

    void Shutdown(Object* obj);
};
