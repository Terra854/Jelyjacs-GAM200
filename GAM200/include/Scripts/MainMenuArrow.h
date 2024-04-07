#pragma once
/* !
@author Yeo Jia Ming
@date	23/3/2024

This file contains the script for the main menu arrows
*//*__________________________________________________________________________*/
#include <Object.h>
#include <GameLogic.h>

class MenuArrow : public LogicScript
{
public:

    MenuArrow(std::string name);

    ~MenuArrow() = default;


    void Start(Object* obj);

    void Update(Object* obj);

 
    void Shutdown(Object* obj);
};
