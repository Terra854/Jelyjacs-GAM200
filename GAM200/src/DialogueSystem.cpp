#pragma once
#include <Debug.h>
#include "DialogueSystem.h"
#include <Factory.h>
#include <iostream>
#include <../components/Text.h>
#include <../components/Transform.h>
#include "Utils.h"
#include <Core_Engine.h>
#include <message.h>
#include <../Scripts/ChatBoxLine1.h>


DialogueSystem* dialoguesystem = NULL;
DialogueSystem::DialogueSystem() {
}


void DialogueSystem::Initialize()
{
	
}




void DialogueSystem::Update()
{
	if (engine->isPaused() && engine->debug_gui_active)
		return;
	

	for (Factory::objectIDMap::iterator obj = objectFactory->objectMap.begin(); obj != objectFactory->objectMap.end(); ++obj)
	{

		Dialogue* d = (Dialogue*)obj->second->GetComponent(ComponentType::Dialogue);
		if (d == nullptr)
		{
			continue; // No Dialogue in this object, move to next object
		}
		if (d->GetSequence() == d->GetDialogueLines().size())
		{
			continue;//dialogue has finished
		}
		Object* finn_obj = objectFactory->FindObject("Finn");
		Transform* finn_trans = (Transform*)finn_obj->GetComponent(ComponentType::Transform);
		Transform* t = (Transform*)obj->second->GetComponent(ComponentType::Transform);
		if (!isObjectClicked(t, finn_trans->Position))
		{
			continue;
		}
		if (next_dialogue)
		{
			next_dialogue = false;
			d->SetSequence(d->GetSequence() + 1);
			std::cout << d->GetEntireDialogue() << std::endl;
			std::cout << "sequence: " << d->GetSequence() << std::endl;
			std::cout << "sequence size: " << d->GetDialogueLines().size() << std::endl;
			std::cout << "\n\n\n\n" << std::endl;
			if (d->GetSequence() == d->GetDialogueLines().size())
			{
				objectFactory->GetLayer("ChatBoxMenu")->second.first.isVisible = false;
				Message_Handler msg(MessageID::Event_Type::Movement);
				engine->Broadcast(&msg);
				continue;
			}
		}
		
		
		
		CHATBOX::change_text(d->GetDialogueLines().at(d->GetSequence()));
		objectFactory->GetLayer("ChatBoxMenu")->second.first.isVisible = true;
		Message_Handler msg(MessageID::Event_Type::NoMovement);
		engine->Broadcast(&msg);
	}
}
