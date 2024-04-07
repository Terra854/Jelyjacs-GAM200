/* !
@author Yeo Jia Ming
@date	23/3/2024

This file contains the script for the main menu how to play button
*//*__________________________________________________________________________*/
#include "Scripts/MainMenuHowToPlay.h"
#include <Utils.h>
#include <Audio.h>
#include <Factory.h>
#include <../components/Texture.h>


MainMenuHowToPlay::MainMenuHowToPlay(std::string name) : LogicScript(name)
{
    std::cout << name << " Created" << std::endl;
}

void MainMenuHowToPlay::Start(Object*) {
    executeOnPause = true;
}


void MainMenuHowToPlay::Update(Object* obj) {
    if (obj == nullptr || !objectFactory->FindLayerThatHasThisObject(obj)->second.first.isVisible) {
        return;
    }

    Transform* trans = (Transform*)obj->GetComponent(ComponentType::Transform);

    if (!isObjectClicked(trans, Vec2(input::GetMouseX(), input::GetMouseY())))
    {
        return;
    }

    if (input::MouseClickedOnce())
    {
        audio->playSfx("button_click");
        objectFactory->GetLayer("HowToPlayMenu")->second.first.isVisible = true;
        objectFactory->GetLayer("MainMenu")->second.first.isVisible = false;
           
    }




    

}

void MainMenuHowToPlay::Shutdown(Object* obj) {
    std::cout << "MainMenuHowToPlay Script Shutdown : " << obj->GetName() << std::endl;
}

MainMenuHowToPlay mainmenuhowtoplay("MainMenuHowToPlay");


