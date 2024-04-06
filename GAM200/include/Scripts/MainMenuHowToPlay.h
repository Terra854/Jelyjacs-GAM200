#pragma once

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
