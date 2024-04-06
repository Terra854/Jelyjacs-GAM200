
#include "Scripts/SoundToggle.h"
#include <Scripts/SoundBar.h>
#include <Utils.h>
#include <Audio.h>
#include <Factory.h>
#include <../components/Texture.h>

bool sound_on = true;

// Constructor for the ButtonBase class.
// @param name: The name of the portal.
SoundToggle::SoundToggle(std::string name) : LogicScript(name)
{
    std::cout << name << " Created" << std::endl;
}
/*********************************************************************/
// Start method called when the ButtonBase script is ready.
// @param obj: The object to which this script is attached.
/*********************************************************************/
void SoundToggle::Start(Object* obj) {
    executeOnPause = true;
    std::cout << "SoundToggle Script Ready : " << obj->GetName() << std::endl;
}

/*********************************************************************/
// Update method called every frame to update the ButtonBase's logic.
// @param obj: The object to which this script is attached.
/*********************************************************************/
void SoundToggle::Update(Object* obj) {
    if (obj == nullptr || objectFactory->FindLayerThatHasThisObject(obj) == nullptr || !objectFactory->FindLayerThatHasThisObject(obj)->second.first.isVisible) {
        //std::cout << "NIL OBJ : ButtonBase" << std::endl;
        return;
    }
    Texture* t = (Texture*)obj->GetComponent(ComponentType::Texture);
    if (!isObjectClicked((Transform*)obj->GetComponent(ComponentType::Transform), Vec2(input::GetMouseX(), input::GetMouseY())))
    {
        return;
    }

    Logic->button_hover = true;

    if (input::IsPressed(KEY::mouseL))
    {
        sound_on = !sound_on;
        t->textureName = sound_on ? "sound_on.png" : "sound_off.png";
        audio->setMasterVolume(sound_on ? sound_volume : 0.0f);
        audio->playSfx("button_click");
    }

}
/*********************************************************************/
// Shutdown method called when the ButtonBase script is being shut down.
// @param obj: The object to which this script is attached.
/*********************************************************************/
void SoundToggle::Shutdown(Object* obj) {
    std::cout << "SoundToggle Script Shutdown : " << obj->GetName() << std::endl;
}

// Creating an instance of ButtonBase.
SoundToggle soundtoggle ("SoundToggle");


