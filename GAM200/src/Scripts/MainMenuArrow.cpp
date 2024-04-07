/* !
@author Yeo Jia Ming
@date	23/3/2024

This file contains the script for the main menu arrows
*//*__________________________________________________________________________*/
#include "Scripts/MainMenuArrow.h"
#include <Utils.h>
#include <Audio.h>
#include <Factory.h>
#include <../components/Texture.h>


MenuArrow::MenuArrow(std::string name) : LogicScript(name)
{
    std::cout << name << " Created" << std::endl;
}

void MenuArrow::Start(Object*) {
    executeOnPause = true;
}


void MenuArrow::Update(Object* obj) {
    if (obj == nullptr || !objectFactory->FindLayerThatHasThisObject(obj)->second.first.isVisible) {
        return;
    }
    Transform* trans = (Transform*)obj->GetComponent(ComponentType::Transform);

    std::vector<Object*> all_objs = objectFactory->FindLayerThatHasThisObject(obj)->second.second;
    for (Object* o : all_objs)
    {
        if (o->GetName() == "MainMenuArrowLeft" || o->GetName() == "MainMenuArrowRight" || o->GetName() == "Main_MenuTitle")
        {
            continue;
        }
        Transform* obj_trans = (Transform*)o->GetComponent(ComponentType::Transform);
        
        if (isObjectClicked(obj_trans, Vec2(input::GetMouseX(), input::GetMouseY())))
        {
            Logic->button_hover = true;
            if (obj->GetName() == "MainMenuArrowRight")
            {
                trans->Position.x = obj_trans->Position.x + obj_trans->Scale.x / 1.8f + 8;
                trans->Position.y = obj_trans->Position.y + 3;
            }
            else if (obj->GetName() == "MainMenuArrowLeft")
            {
                trans->Position.x = obj_trans->Position.x - obj_trans->Scale.x / 1.8f;
                trans->Position.y = obj_trans->Position.y + 3;
            }
        }
    }

}

void MenuArrow::Shutdown(Object* obj) {
    std::cout << "MainMenuArrow Script Shutdown : " << obj->GetName() << std::endl;
}


MenuArrow menuarrow("MainMenuArrow");


