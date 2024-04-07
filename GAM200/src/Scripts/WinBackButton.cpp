/* !
@author Yeo Jia Ming
@date	1/3/2024

This file contains the script for the win screen button
*//*__________________________________________________________________________*/
#include "Scripts/WinBackButton.h"
#include <Utils.h>
#include <Audio.h>
#include <Factory.h>
#include <Core_Engine.h>
#include <SceneManager.h>
#include <../components/Texture.h>
#include <Factory.h>

WinBackButton::WinBackButton(std::string name) : LogicScript(name)
{
    std::cout << name << " Created" << std::endl;
}

void WinBackButton::Start(Object* obj) {
    executeOnPause = true;
    std::cout << "WinBackButton Script Ready : " << obj->GetName() << std::endl;
}


void WinBackButton::Update(Object* obj) {
    if (obj == nullptr || !objectFactory->FindLayerThatHasThisObject(obj) || !objectFactory->FindLayerThatHasThisObject(obj)->second.first.isVisible) {
        //std::cout << "NIL OBJ : WinBackButton" << std::endl;
        return;
    }

    Texture* texture = (Texture*)obj->GetComponent(ComponentType::Texture);

    if(!isObjectClicked((Transform*)obj->GetComponent(ComponentType::Transform), Vec2(input::GetMouseX(), input::GetMouseY())))
    {
        texture->textureName = "win_back_button.png";
        return;
    }
    
    texture->textureName = "win_back_button_glow.png";

    Logic->button_hover = true;
    

    if (input::IsPressed(KEY::mouseL))
    {
        audio->playSfx("button_click");
        //objectFactory->GetLayer("WinMenu")->second.first.isVisible = false;
        //sceneManager->RestartScene();
        audio->setBackgroundAudio("main_menu_bg");
        sceneManager->LoadScene("main_menu.json");
        sceneManager->PlayScene();
    }
}

void WinBackButton::Shutdown(Object* obj) {
    std::cout << "WinBackButton Script Shutdown : " << obj->GetName() << std::endl;
}

WinBackButton winbackButton ("WinBackButton");


