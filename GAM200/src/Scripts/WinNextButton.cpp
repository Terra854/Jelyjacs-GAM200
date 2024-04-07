/* !
@author Yeo Jia Ming
@date	1/3/2024

This file contains the script for the win screen button
*//*__________________________________________________________________________*/
#include "Scripts/WinNextButton.h"
#include <Utils.h>
#include <Audio.h>
#include <Factory.h>
#include <Core_Engine.h>
#include <SceneManager.h>
#include <../components/Texture.h>
#include <Factory.h>

WinNextButton::WinNextButton(std::string name) : LogicScript(name)
{
    std::cout << name << " Created" << std::endl;
}

void WinNextButton::Start(Object* obj) {
    executeOnPause = true;
    std::cout << "WinBackButton Script Ready : " << obj->GetName() << std::endl;
}


void WinNextButton::Update(Object* obj) {
    if (obj == nullptr || !objectFactory->FindLayerThatHasThisObject(obj) || !objectFactory->FindLayerThatHasThisObject(obj)->second.first.isVisible) {
        //std::cout << "NIL OBJ : WinBackButton" << std::endl;
        return;
    }

    Texture* texture = (Texture*)obj->GetComponent(ComponentType::Texture);

    if(!isObjectClicked((Transform*)obj->GetComponent(ComponentType::Transform), Vec2(input::GetMouseX(), input::GetMouseY())))
    {
        texture->textureName = "win_next_button.png";
        return;
    }
    texture->textureName = "win_next_button_glow.png";
    Logic->button_hover = true;

    

    if (input::IsPressed(KEY::mouseL))
    {
        audio->playSfx("button_click");
        objectFactory->GetLayer("WinMenu")->second.first.isVisible = false;

        audio->restartBackgroundAudio();

        if (engine->loaded_level == "tutorial_level")
            sceneManager->LoadScene("level_1.json");
        else if (engine->loaded_level == "level_1")
            sceneManager->LoadScene("level_2.json");
        else if (engine->loaded_level == "level_2") {
            sceneManager->LoadScene("ending_cutscene.json");
        }
        sceneManager->PlayScene();
    }
}

void WinNextButton::Shutdown(Object* obj) {
    std::cout << "WinNextButton Script Shutdown : " << obj->GetName() << std::endl;
}

WinNextButton winnextButton ("WinNextButton");


