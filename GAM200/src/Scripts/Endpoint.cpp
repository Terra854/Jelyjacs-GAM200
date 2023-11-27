/* !
@file	Endpoint.cpp
@author Luke Goh
@date	27/11/2023

This file contains the script for the endpoint door
*//*__________________________________________________________________________*/
#include "Scripts/Endpoint.h"
#include <iostream>
#include <components/Transform.h>
#include <components/Animation.h>
#include "gamehud.h"

Endpoint::Endpoint(std::string name) : LogicScript(name)
{
	std::cout << name << " Created" << std::endl;
}


void Endpoint::Start(Object* obj) {
	std::cout << "Endpoint Script Ready : " << obj->GetName() << std::endl;
}

void Endpoint::Update(Object* obj) {
	if (obj == nullptr || GameLogic::playerObj == nullptr) {
		//std::cout << "NIL OBJ : Endpoint" << std::endl;
		return;
	}
	

	Object* other_player = nullptr;
	if (GameLogic::playerObj == objectFactory->FindObject("Spark")) {
		other_player = objectFactory->FindObject("Finn");
	}
	else if (GameLogic::playerObj == objectFactory->FindObject("Finn")) {
		other_player = objectFactory->FindObject("Spark");
	}
	else
	{
		std::cout << "NIL OTHER PLAYER : Endpoint" << std::endl;
		return;
	}
	//Check if both characters is in the endpoint
	Transform* player_t = static_cast<Transform*>(GameLogic::playerObj->GetComponent(ComponentType::Transform));
	Transform* other_player_t = static_cast<Transform*>(other_player->GetComponent(ComponentType::Transform));

	Transform* endpoint_t = static_cast<Transform*>(obj->GetComponent(ComponentType::Transform));
	Animation* endpoint_animation = static_cast<Animation*>(obj->GetComponent(ComponentType::Animation));
	if (player_t == nullptr || endpoint_t == nullptr) {
		std::cout << "NIL COMPONENT : Endpoint" << std::endl;
		return;
	};
	if (player_t->Position.x > endpoint_t->Position.x - 50 && player_t->Position.x < endpoint_t->Position.x + 50) {
		if (player_t->Position.y > endpoint_t->Position.y - 50 && player_t->Position.y < endpoint_t->Position.y + 50) {
			if (other_player_t->Position.x > endpoint_t->Position.x - 50 && other_player_t->Position.x < endpoint_t->Position.x + 50) {
				if (other_player_t->Position.y > endpoint_t->Position.y - 50 && other_player_t->Position.y < endpoint_t->Position.y + 50) {
					//std::cout << "Both player is in the endpoint" << std::endl;
					//Get Animation and change the animation to jump
					endpoint_animation->fixed = true;
					endpoint_animation->current_type = AnimationType::Jump;
					set_win();
				}
			}
		}
	}
	else {
		endpoint_animation->fixed = false;
		endpoint_animation->current_type = AnimationType::Idle;
	}
}

void Endpoint::Shutdown(Object* obj) {
	std::cout << "Endpoint Script Shutdown : " << obj->GetName() << std::endl;
}

Endpoint endpoint("Endpoint");