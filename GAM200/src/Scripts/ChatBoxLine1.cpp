#pragma once
/* !
@author Yeo Jia Ming
@date	4/4/2024

This file contains the script for chatbox text alignment
*//*__________________________________________________________________________*/
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


namespace CHATBOX
{
    void change_text(std::string);
    bool align_text = false;
}

namespace
{
    bool aligned = false;
    bool change_text = false;
    std::string dialogue_line;
}


ChatBoxLine1::ChatBoxLine1(std::string name) : LogicScript(name)
{
    std::cout << name << " Created" << std::endl;
}

void ChatBoxLine1::Start(Object* obj) {
    executeOnPause = true;
    std::cout << "ChatBoxline1 Script Ready : " << obj->GetName() << std::endl;
    
}


void ChatBoxLine1::Update(Object* obj) {
    if (obj == nullptr || !objectFactory->FindLayerThatHasThisObject(obj) || !objectFactory->FindLayerThatHasThisObject(obj)->second.first.isVisible) {
        return;
    }
    Text* text_obj = (Text*)obj->GetComponent(ComponentType::Text);
    text_obj->text = ::dialogue_line;
 
    ChatBoxAlignment(obj);
    


}

void ChatBoxLine1::Shutdown(Object* obj) {
    std::cout << "ChatBoxline1 Script Shutdown : " << obj->GetName() << std::endl;
}


ChatBoxLine1 chatboxline1 ("ChatBoxLine1");


void ChatBoxAlignment(Object* obj)
{
    Transform* line_trans = (Transform*)obj->GetComponent(ComponentType::Transform);
    //get chatbox_bg
    Object* chatbox = objectFactory->FindObject("ChatBox_bg");
    Transform* chatbox_trans = (Transform*)chatbox->GetComponent(ComponentType::Transform);

    //align chatboxline to chatbox_bg
    Vec2 top_left = { chatbox_trans->Position.x - chatbox_trans->Scale.x * 0.8f / 2.0f , chatbox_trans->Position.y + chatbox_trans->Scale.y * 0.2f / 2.f };
    Vec2 top_right = { chatbox_trans->Position.x + chatbox_trans->Scale.x * 0.6f / 2.0f , chatbox_trans->Position.y + chatbox_trans->Scale.y * 0.2f / 2.f };
    line_trans->Scale.x = top_right.x - top_left.x;
    line_trans->Position.x = top_left.x + line_trans->Scale.x / 2.0f;
    line_trans->Position.y = top_left.y;
}

void CHATBOX::change_text(std::string str)
{
    ::dialogue_line = str;
    ::change_text = true;
}

