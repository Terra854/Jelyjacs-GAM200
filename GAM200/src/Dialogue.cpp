
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
    for (std::string::size_type pos1{}, pos2{ 1 }; pos2 != std::string::npos; )
    {
        pos2 = entire_dialogue.find_first_of('\r', pos1);
        std::string line{ entire_dialogue.substr(pos1 , pos2) };
        pos1 = pos2 + 1;
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
