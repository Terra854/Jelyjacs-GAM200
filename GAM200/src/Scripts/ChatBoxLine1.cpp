
#include "Scripts/ChatBoxline1.h"
#include <Utils.h>
#include <Audio.h>
#include <Factory.h>
#include <Core_Engine.h>
#include <SceneManager.h>
#include <../components/Texture.h>
#include <../components/Text.h>
#include <Factory.h>
#include <Font.h>


// @param name: The name of the portal.
ChatBoxLine1::ChatBoxLine1(std::string name) : LogicScript(name)
{
    std::cout << name << " Created" << std::endl;
}
/*********************************************************************/
// Start method called when the PauseButton script is ready.
// @param obj: The object to which this script is attached.
/*********************************************************************/
void ChatBoxLine1::Start(Object* obj) {
    executeOnPause = true;
    std::cout << "ChatBoxline1 Script Ready : " << obj->GetName() << std::endl;
}

/*********************************************************************/
// Update method called every frame to update the PauseButton's logic.
// @param obj: The object to which this script is attached.
/*********************************************************************/
void ChatBoxLine1::Update(Object* obj) {
    if (obj == nullptr || !objectFactory->FindLayerThatHasThisObject(obj) || !objectFactory->FindLayerThatHasThisObject(obj)->second.first.isVisible) {
        return;
    }

    Text* text = (Text*)obj->GetComponent(ComponentType::Text);

    Transform* trans = (Transform*)obj->GetComponent(ComponentType::Transform);
    trans->Scale.x = find_width(text->text, text->fontType, 0.8);
    trans->Scale.y = find_height(text->text, text->fontType, 0.8);
    Object* chatbox = objectFactory->FindObject("ChatBox_bg");

    Transform* chatbox_trans = (Transform*)chatbox->GetComponent(ComponentType::Transform);
    Vec2 top_left = { chatbox_trans->Position.x - chatbox_trans->Scale.x * 0.8f / 2.0f , chatbox_trans->Position.y };
    trans->Position.x = top_left.x + trans->Scale.x / 2.0f;
    trans->Position.y = top_left.y;





}
/*********************************************************************/
// Shutdown method called when the PauseButton script is being shut down.
// @param obj: The object to which this script is attached.
/*********************************************************************/
void ChatBoxLine1::Shutdown(Object* obj) {
    std::cout << "ChatBoxline1 Script Shutdown : " << obj->GetName() << std::endl;
}

// Creating an instance of PauseButton.
ChatBoxLine1 chatboxline1 ("ChatBoxLine1");


