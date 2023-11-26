/* !
@file GameLogic.cpp
@author Luke Goh
@date	27/11/2023

This file contains the script for the portal for the cat character to teleport
*//*__________________________________________________________________________*/
#include "Scripts/CatPortal.h"
#include <components/Body.h>
#include <components/Animation.h>
#include <components/Event.h>
#include <Audio.h>

bool CatPortal::justTeleported = false;
CatPortal::CatPortal(std::string name) : LogicScript(name)
{
	std::cout << name << " Created" << std::endl;
}

void CatPortal::Start(Object* obj) {
	std::cout << "CatPortal Script Ready : " << obj->GetName() << std::endl;
}

void CatPortal::Update(Object* obj) {
	// For some reason, the player is not changing position
	if (obj == nullptr) {
		//std::cout << "NIL OBJ : CatPortal" << std::endl;
		return;
	}
	
	Animation* catPortal_animation = static_cast<Animation*>(obj->GetComponent(ComponentType::Animation));
	//catPortal_animation->fixed = true;
	//catPortal_animation->current_type = AnimationType::Jump;
	
	Object* Checkplayer = objectFactory->FindObject("Spark");
	if (GameLogic::playerObj != nullptr && GameLogic::playerObj == Checkplayer)
	{
		Transform* player_t = static_cast<Transform*>(GameLogic::playerObj->GetComponent(ComponentType::Transform));
		Transform* catPortal_t = static_cast<Transform*>(obj->GetComponent(ComponentType::Transform));
		Object* otherCatPortal = nullptr;
		Transform* otherCatPortal_t = nullptr;
		for (int i = 0; i < objectFactory->NumberOfObjects(); i++)
		{
			Object* temp_obj = objectFactory->getObjectWithID((long)i);
			if (temp_obj == nullptr)
				return;
			if (static_cast<Behaviour*>(temp_obj->GetComponent(ComponentType::Behaviour)) != nullptr) {
				if (static_cast<Behaviour*>(temp_obj->GetComponent(ComponentType::Behaviour))->GetBehaviourName() == "CatPortal" && temp_obj != obj)
				{
					otherCatPortal = temp_obj;
					otherCatPortal_t = static_cast<Transform*>(otherCatPortal->GetComponent(ComponentType::Transform));
				}
			}
		}
		if (player_t == nullptr || catPortal_t == nullptr) {
			std::cout << "NIL COMPONENT : CatPortal" << std::endl;
			return;
		}
		if (otherCatPortal == nullptr || otherCatPortal_t == nullptr) {
			std::cout << "NIL OTHER CAT PORTAL : CatPortal" << std::endl;
			return;
		}
		if (player_t->Position.x > catPortal_t->Position.x - 50 && player_t->Position.x < catPortal_t->Position.x + 50) {
			if (player_t->Position.y > catPortal_t->Position.y - 50 && player_t->Position.y < catPortal_t->Position.y + 50) {
				if (justTeleported == true)
					return;
				else {
					std::cout << "Player is in the cat portal" << std::endl;
					//Find the other cat portal
					if (otherCatPortal_t == nullptr) {
						std::cout << "NIL OTHER CAT PORTAL TRANSFORM : CatPortal" << std::endl;
						return;
					}
					audio->playSfx(AudioType::Cat_Teleport);
					player_t->Position.x = otherCatPortal_t->Position.x;
					player_t->Position.y = otherCatPortal_t->Position.y;
					//std::cout << "Player position changed to the other cat portal's position" << std::endl;
					justTeleported = true;
						
					
				}
			}
			
		}
		else if ((player_t->Position.x > otherCatPortal_t->Position.x - 50 && player_t->Position.x < otherCatPortal_t->Position.x + 50)
			&& (player_t->Position.y > otherCatPortal_t->Position.y - 50 && player_t->Position.y < otherCatPortal_t->Position.y + 50))
		{
			
		}
		else {
			if (catPortal_animation != nullptr) {
				catPortal_animation->fixed = false;
				catPortal_animation->current_type = AnimationType::Idle;
			}
			justTeleported = false;
		}
		
	}
}

void CatPortal::Shutdown(Object* obj) {
	std::cout << "CatPortal Script Shutdown : " << obj->GetName() << std::endl;
}

CatPortal catPortal("CatPortal");