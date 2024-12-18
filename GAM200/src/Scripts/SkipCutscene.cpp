/* !
@file	SkipCutscene.cpp
@author Tan Yee Ann (t.yeeann@digipen.edu)
@date	26/2/2023

This file contains the script for the in-game clickable buttons
*//*__________________________________________________________________________*/
#include "Scripts/SkipCutscene.h"
#include <Utils.h>
#include <Audio.h>
#include <Factory.h>
#include <SceneManager.h>
#include "Scripts/OpeningCutscene.h"

// Constructor for the SkipCutscene class.
// @param name: The name of the portal.
SkipCutscene::SkipCutscene(std::string name) : LogicScript(name)
{
    std::cout << name << " Created" << std::endl;
}
/*********************************************************************/
// Start method called when the SkipCutscene script is ready.
// @param obj: The object to which this script is attached.
/*********************************************************************/
void SkipCutscene::Start(Object* obj) {
    executeOnPause = true;
    std::cout << "SkipCutscene Script Ready : " << obj->GetName() << std::endl;
}

/*********************************************************************/
// Update method called every frame to update the SkipCutscene's logic.
// @param obj: The object to which this script is attached.
/*********************************************************************/
void SkipCutscene::Update(Object* obj) {
    if (obj == nullptr || objectFactory->FindLayerThatHasThisObject(obj) == nullptr || !objectFactory->FindLayerThatHasThisObject(obj)->second.first.isVisible) {
        //std::cout << "NIL OBJ : PauseButton" << std::endl;
        return;
    }

    if (isObjectClicked((Transform*)obj->GetComponent(ComponentType::Transform), Vec2(input::GetMouseX(), input::GetMouseY()))) {
        Logic->button_hover = true;

        if (input::IsPressed(KEY::mouseL)) {
            std::cout << "Button Clicked" << std::endl;
            audio->playSfx("button_click");

            // Skip the cutscene
            audio->setBackgroundAudio("main_menu_bg");
            audio->stopSfx(rain_sfx);
            audio->stopSfx(people_sfx);
            SceneManager::LoadScene("main_menu.json");
        }
	}
}
/*********************************************************************/
// Shutdown method called when the SkipCutscene script is being shut down.
// @param obj: The object to which this script is attached.
/*********************************************************************/
void SkipCutscene::Shutdown(Object* obj) {
    std::cout << "SkipCutscene Script Shutdown : " << obj->GetName() << std::endl;
}

// Creating an instance of SkipCutscene.
SkipCutscene SkipCutscene("SkipCutscene");


