#pragma once
/* !
@author Yeo Jia Ming
@date	23/3/2024

This file contains the script for the main menu how to play button
*//*__________________________________________________________________________*/
#include <Object.h>
#include <GameLogic.h>

class MainMenuHowToPlay : public LogicScript
{
public:
    MainMenuHowToPlay(std::string name);

    ~MainMenuHowToPlay() = default;

    void Start(Object* obj);

    void Update(Object* obj);

    void Shutdown(Object* obj);
};
