/* !
@author Yeo Jia Ming
@date	23/3/2024

This file contains the component for dialogue
*//*__________________________________________________________________________*/
#include "../include/components/Dialogue.h"
#include <iostream>
#include <../components/Transform.h>
#include <../components/Text.h>
#include <Font.h>
#include <Factory.h>


Dialogue::Dialogue()
	: Component(){}

void Dialogue::Initialize()
{
    dialogue_lines.clear();
    if (entire_dialogue.size() == 0)
    {
        return;
    }

    std::string::size_type pos1{};
    std::string::size_type pos2{};
    while (entire_dialogue.find_first_of('[', pos1) != std::string::npos)
    {
        pos1 = entire_dialogue.find_first_of('[', pos1) + 1;
        pos2 = entire_dialogue.find_first_of(']', pos1);
        if (pos2 == std::string::npos)
        {
            break;
        }
        std::string line{ entire_dialogue.substr(pos1,pos2 - pos1) };
        dialogue_lines.push_back(line);
    }
}


std::vector<std::string> Dialogue::GetDialogueLines() const
{
    return dialogue_lines;
}

void Dialogue::ChangeDialogue(std::string new_dialogue)
{
    sequence = 0;
    entire_dialogue = new_dialogue;
    Initialize();
    //CHATBOX::align_text = true;
}


int Dialogue::GetSequence() const
{
    return sequence;
}
void Dialogue::SetSequence(int x)
{
    sequence = x;
}

std::string Dialogue::GetEntireDialogue() const
{
    return entire_dialogue;
}
