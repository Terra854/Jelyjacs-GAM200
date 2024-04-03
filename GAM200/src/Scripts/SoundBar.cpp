
#include "Scripts/SoundBar.h"
#include <Scripts/SoundToggle.h>
#include <Utils.h>
#include <Audio.h>
#include <Factory.h>


Object* sound_bg_obj;
Transform* sound_bg_trans;
float sound_volume = 1.0f;

// Constructor for the ButtonBase class.
// @param name: The name of the portal.
SoundBar::SoundBar(std::string name) : LogicScript(name)
{
    std::cout << name << " Created" << std::endl;
}
/*********************************************************************/
// Start method called when the ButtonBase script is ready.
// @param obj: The object to which this script is attached.
/*********************************************************************/
void SoundBar::Start(Object* obj) {
    executeOnPause = true;
    std::cout << "SoundBar Script Ready : " << obj->GetName() << std::endl;
    sound_bg_obj = objectFactory->FindObject("SoundBar_bg");
    
}

/*********************************************************************/
// Update method called every frame to update the ButtonBase's logic.
// @param obj: The object to which this script is attached.
/*********************************************************************/
void SoundBar::Update(Object* obj) {
    if (obj == nullptr || objectFactory->FindLayerThatHasThisObject(obj) == nullptr || !objectFactory->FindLayerThatHasThisObject(obj)->second.first.isVisible) {
        return;
    }
    sound_bg_trans = (Transform*)sound_bg_obj->GetComponent(ComponentType::Transform);
    Transform* sound_bar_trans = (Transform*)obj->GetComponent(ComponentType::Transform);

    if (!sound_on)
    {
        sound_bg_trans->Scale.x = 0;
    }
    else
    {
        sound_bg_trans->Scale.x = sound_volume * sound_bar_trans->Scale.x;
    }



    if (!isObjectClicked((Transform*)obj->GetComponent(ComponentType::Transform), Vec2(input::GetMouseX(), input::GetMouseY()))) {
        return;
    }


    if (!sound_bg_obj)
    {
        std::cout << "object does not exist" << std::endl;
        return;
    }
    

   
    

    
    float pos1 = sound_bar_trans->Position.x - sound_bar_trans->Scale.x / 2.0f;
    if (input::IsPressedRepeatedly(KEY::mouseL))
    {
        sound_bg_trans->Scale.x = input::GetMouseX() - pos1;
        sound_bg_trans->Position.x = pos1 + sound_bg_trans->Scale.x/2.0f;
        audio->setMasterVolume(sound_bg_trans->Scale.x / sound_bar_trans->Scale.x);
        sound_volume = sound_bg_trans->Scale.x / sound_bar_trans->Scale.x;
        sound_on = true;
    }
    
    

}
/*********************************************************************/
// Shutdown method called when the ButtonBase script is being shut down.
// @param obj: The object to which this script is attached.
/*********************************************************************/
void SoundBar::Shutdown(Object* obj) {
    std::cout << "SoundBar Script Shutdown : " << obj->GetName() << std::endl;
}

// Creating an instance of ButtonBase.
SoundBar soundbar("SoundBar");

