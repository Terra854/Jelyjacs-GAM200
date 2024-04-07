#pragma once
/* !
@author Yeo Jia Ming
@date	1/3/2024

This file contains the script for the win screen button
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
