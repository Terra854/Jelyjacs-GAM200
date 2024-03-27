
#include "../include/components/Dialogue.h"
#include <iostream>

Dialogue::Dialogue()
	: Component(){}

void Dialogue::Initialize()
{
    dialogue_lines.clear();
    for (std::string::size_type pos1{}, pos2{ 1 }; pos2 != std::string::npos; )
    {
        pos2 = entire_dialogue.find_first_of('\r', pos1);
        std::string line{ entire_dialogue.substr(pos1 , pos2) };
        pos1 = pos2 + 1;
        dialogue_lines.push_back(line);
    }
}


