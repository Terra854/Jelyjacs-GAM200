#pragma once
/* !
@author Yeo Jia Ming
@date	10/3/2024

This file contains the script for fullscreen button
*//*__________________________________________________________________________*/
#include <Object.h>
#include <GameLogic.h>

class FullscreenButton : public LogicScript
{
public:
    FullscreenButton(std::string name);

    ~FullscreenButton() = default;

    void Start(Object* obj);

    void Update(Object* obj);

    void Shutdown(Object* obj);
};
