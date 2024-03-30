
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


void ChatBoxAlignment(Object* obj);
void TextAlignment(Object* obj);

namespace
{
    bool aligned = false;
}

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
    if (!aligned)
    {
        ChatBoxAlignment(obj);
        aligned = true;
    }
    if (CHATBOX::align_text)
    {
        TextAlignment(obj);
        CHATBOX::align_text = false;
    }
    TextAlignment(obj);


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


void ChatBoxAlignment(Object* obj)
{
    //get chatbox_bg
    Transform* line_trans = (Transform*)obj->GetComponent(ComponentType::Transform);
    Object* chatbox = objectFactory->FindObject("ChatBox_bg");
    Transform* chatbox_trans = (Transform*)chatbox->GetComponent(ComponentType::Transform);

    //align chatboxline to chatbox_bg
    Vec2 top_left = { chatbox_trans->Position.x - chatbox_trans->Scale.x * 0.8f / 2.0f , chatbox_trans->Position.y + chatbox_trans->Scale.y * 0.2f / 2.f };
    Vec2 top_right = { chatbox_trans->Position.x + chatbox_trans->Scale.x * 0.6f / 2.0f , chatbox_trans->Position.y + chatbox_trans->Scale.y * 0.2f / 2.f };
    line_trans->Scale.x = top_right.x - top_left.x;
    line_trans->Position.x = top_left.x + line_trans->Scale.x / 2.0f;
    line_trans->Position.y = top_left.y;
}

void TextAlignment(Object* obj)
{
    Text* text_obj = (Text*)obj->GetComponent(ComponentType::Text);
    Transform* trans = (Transform*)obj->GetComponent(ComponentType::Transform);
    int text_width = find_width(text_obj->text , text_obj->fontType , text_obj->fontSize);
    int text_height = find_height(text_obj->text, text_obj->fontType, text_obj->fontSize);
   
    std::string::size_type pos1{ text_obj->text.size() - 1 };
    int new_text_width{ text_width };
    for (; new_text_width > trans->Scale.x; --pos1)
    {
        new_text_width = find_width(text_obj->text.substr(0, pos1), text_obj->fontType, text_obj->fontSize);
    }
    text_obj->text = text_obj->text.substr(0, pos1);
    std::cout << text_obj->text << std::endl;
    std::cout << new_text_width << std::endl;
    std::cout << text_width << std::endl;
}