#pragma once
/* !
@author Yeo Jia Ming
@date	1/3/2024

This file contains the script for the win screen button
*//*__________________________________________________________________________*/
#include <Object.h>
#include <GameLogic.h>

class WinBackButton : public LogicScript
{
public:
    WinBackButton(std::string name);

    ~WinBackButton() = default;

    void Start(Object* obj);

    void Update(Object* obj);

    void Shutdown(Object* obj);
};
