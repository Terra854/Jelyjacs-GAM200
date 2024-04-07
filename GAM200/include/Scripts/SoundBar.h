#pragma once
/* !
@author Yeo Jia Ming
@date	23/3/2024

This file contains the script for the pause menu sound bar
*//*__________________________________________________________________________*/
#include <Object.h>
#include <GameLogic.h>


class SoundBar : public LogicScript
{
public:

    SoundBar(std::string name);

  
    ~SoundBar() = default;

  
    void Start(Object* obj);

  
    void Update(Object* obj);

    void Shutdown(Object* obj);
};