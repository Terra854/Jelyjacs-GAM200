
#include <Debug.h>
#include "DialogueSystem.h"
#include <Factory.h>
#include <iostream>
#include <../components/Text.h>
#include <../components/Transform.h>
#include "Utils.h"
#include <Core_Engine.h>
#include <message.h>


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
		if (next_dialogue)
		{
			next_dialogue = false;
			d->SetSequence(d->GetSequence() + 1);
			if (d->GetSequence() == d->GetDialogueLines().size())
			{
				objectFactory->GetLayer("ChatBoxMenu")->second.first.isVisible = false;
				Message_Handler msg(MessageID::Event_Type::Movement);
				engine->Broadcast(&msg);
				continue;
			}
		}
		Transform* t = (Transform*)obj->second->GetComponent(ComponentType::Transform);
		
		Object* finn_obj = objectFactory->FindObject("Finn");
		Transform * finn_trans = (Transform*)finn_obj->GetComponent(ComponentType::Transform);
		if (!isObjectClicked(t, finn_trans->Position))
		{
			continue;
		}

		Object* chatboxhud_obj = objectFactory->FindObject("ChatBoxline1");

		if (chatboxhud_obj == nullptr)
		{
			std::cout << "no such object!!!!!!!!!" << std::endl;
			return;
		}
		
		Text* chatbox_text = (Text*)chatboxhud_obj->GetComponent(ComponentType::Text);
		chatbox_text->text = d->GetDialogueLines().at(d->GetSequence());
		objectFactory->GetLayer("ChatBoxMenu")->second.first.isVisible = true;
		Message_Handler msg(MessageID::Event_Type::NoMovement);
		engine->Broadcast(&msg);
	}
}
