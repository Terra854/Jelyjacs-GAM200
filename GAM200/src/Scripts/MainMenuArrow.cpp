
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
    Object* start_button = objectFactory->FindObject("main_menuTitleStartButton");
    Object* quit_button = objectFactory->FindObject("main_menuTitleQuitButton");
    Transform* start_button_trans = (Transform*)start_button->GetComponent(ComponentType::Transform);
    Transform* quit_button_trans = (Transform*)quit_button->GetComponent(ComponentType::Transform);
    Texture* tex = (Texture*)obj->GetComponent(ComponentType::Texture);
    Transform * trans = (Transform*)obj->GetComponent(ComponentType::Transform);

    if (isObjectClicked(start_button_trans, Vec2(input::GetMouseX(), input::GetMouseY())))
    {
        tex->opacity = 1.f;
        Logic->button_hover = true;
        if (obj->GetName() == "MainMenuArrowRight")
        {
            tex->textureName = "YellowArrow_Right.png";
            trans->Position.x = start_button_trans->Position.x + start_button_trans->Scale.x/1.8f + 8;
            trans->Position.y = start_button_trans->Position.y + 3;
        }
        else if (obj->GetName() == "MainMenuArrowLeft")
        {
            tex->textureName = "YellowArrow_Left.png";
            trans->Position.x = start_button_trans->Position.x - start_button_trans->Scale.x / 1.8f;
            trans->Position.y = start_button_trans->Position.y + 3;
        }
    }
    else if (isObjectClicked(quit_button_trans, Vec2(input::GetMouseX(), input::GetMouseY())))
    {
        tex->opacity = 1.f;
        Logic->button_hover = true;
        if (obj->GetName() == "MainMenuArrowRight")
        {
            tex->textureName = "YellowArrow_Right.png";
            trans->Position.x = quit_button_trans->Position.x + quit_button_trans->Scale.x/1.5f + 5;
            trans->Position.y = quit_button_trans->Position.y + 5;
        }
        else if (obj->GetName() == "MainMenuArrowLeft")
        {
            tex->textureName = "YellowArrow_Left.png";
            trans->Position.x = quit_button_trans->Position.x - quit_button_trans->Scale.x / 1.5f;
            trans->Position.y = quit_button_trans->Position.y + 5;
        }
    }
    else
    {
        tex->opacity = 0.f;
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


