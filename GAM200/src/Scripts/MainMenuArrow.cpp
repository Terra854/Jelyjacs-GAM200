
#include "Scripts/MainMenuArrow.h"
#include <Utils.h>
#include <Audio.h>
#include <Factory.h>
#include <../components/Texture.h>

// Constructor for the ButtonBase class.
// @param name: The name of the portal.
MenuArrow::MenuArrow(std::string name) : LogicScript(name)
{
    std::cout << name << " Created" << std::endl;
}
/*********************************************************************/
// Start method called when the ButtonBase script is ready.
// @param obj: The object to which this script is attached.
/*********************************************************************/
void MenuArrow::Start(Object* obj) {
    executeOnPause = true;
}

/*********************************************************************/
// Update method called every frame to update the ButtonBase's logic.
// @param obj: The object to which this script is attached.
/*********************************************************************/
void MenuArrow::Update(Object* obj) {
    if (obj == nullptr || !objectFactory->FindLayerThatHasThisObject(obj)->second.first.isVisible) {
        //std::cout << "NIL OBJ : ButtonBase" << std::endl;
        return;
    }
    Transform* trans = (Transform*)obj->GetComponent(ComponentType::Transform);

    std::vector<Object*> all_objs = objectFactory->FindLayerThatHasThisObject(obj)->second.second;
    for (Object* o : all_objs)
    {
        if (o->GetName() == "MainMenuArrowLeft" || o->GetName() == "MainMenuArrowRight")
        {
            continue;
        }
        std::cout << o->GetName() << std::endl;
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
/*********************************************************************/
// Shutdown method called when the ButtonBase script is being shut down.
// @param obj: The object to which this script is attached.
/*********************************************************************/
void MenuArrow::Shutdown(Object* obj) {
    std::cout << "MainMenuArrow Script Shutdown : " << obj->GetName() << std::endl;
}

// Creating an instance of ButtonBase.
MenuArrow menuarrow("MainMenuArrow");


