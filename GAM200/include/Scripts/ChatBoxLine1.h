#pragma once
/* !
@author Yeo Jia Ming
@date	4/4/2024

This file contains the script for chatbox text alignment
*//*__________________________________________________________________________*/
#include <Object.h>
#include <GameLogic.h>

class ChatBoxLine1 : public LogicScript
{
public:

    ChatBoxLine1(std::string name);

    ~ChatBoxLine1() = default;


    void Start(Object* obj);

  
    void Update(Object* obj);

    void Shutdown(Object* obj);


    
};

namespace CHATBOX
{
    extern void change_text(std::string);
    extern bool align_text;
}
