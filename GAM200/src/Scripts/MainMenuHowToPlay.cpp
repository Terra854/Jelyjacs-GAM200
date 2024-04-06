
#include "Scripts/MainMenuHowToPlay.h"
#include <Utils.h>
#include <Audio.h>
#include <Factory.h>
#include <../components/Texture.h>

// Constructor for the ButtonBase class.
// @param name: The name of the portal.
MainMenuHowToPlay::MainMenuHowToPlay(std::string name) : LogicScript(name)
{
    std::cout << name << " Created" << std::endl;
}
/*********************************************************************/
// Start method called when the ButtonBase script is ready.
// @param obj: The object to which this script is attached.
/*********************************************************************/
void MainMenuHowToPlay::Start(Object* obj) {
    executeOnPause = true;
}

/*********************************************************************/
// Update method called every frame to update the ButtonBase's logic.
// @param obj: The object to which this script is attached.
/*********************************************************************/
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
/*********************************************************************/
// Shutdown method called when the ButtonBase script is being shut down.
// @param obj: The object to which this script is attached.
/*********************************************************************/
void MainMenuHowToPlay::Shutdown(Object* obj) {
    std::cout << "MainMenuHowToPlay Script Shutdown : " << obj->GetName() << std::endl;
}

// Creating an instance of ButtonBase.
MainMenuHowToPlay mainmenuhowtoplay("MainMenuHowToPlay");


