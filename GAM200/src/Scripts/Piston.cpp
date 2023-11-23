#include "Scripts/Piston.h"
#include <Factory.h>
#include <Object.h>
#include <GameLogic.h>
#include <components/Body.h>
#include <components/Transform.h>
#include <components/Animation.h>
#include <components/Event.h>
#include <iostream>
#include <Audio.h>


Piston::Piston(std::string name) : LogicScript(name)
{
	std::cout << name << " Created" << std::endl;
}

void Piston::Start(Object* obj) {
	std::cout << "Piston Script Ready : " << obj->GetName() << std::endl;
}

void Piston::Update(Object* obj) {
	// For some reason, the player is not changing position
	if (obj == nullptr) {
		std::cout << "NIL OBJ : Piston" << std::endl;
		return;
	}
	Rectangular* piston_b = static_cast<Rectangular*>(obj->GetComponent(ComponentType::Body));
	//Transform* player_t = static_cast<Transform*>(playerObj->GetComponent(ComponentType::Transform));
	if (piston_b == nullptr) {
		std::cout << "NIL BODY : Piston" << std::endl;
		return;
	};
	// if piston collides with player, change the animation of piston
	if (piston_b->collision_flag & COLLISION_TOP) {
		std::cout << "Piston Collision" << std::endl;
		Animation* piston_animation = static_cast<Animation*>(obj->GetComponent(ComponentType::Animation));
		piston_animation->fixed = true;
		if (piston_animation->current_type == AnimationType::Jump){}
		else {
			piston_animation->current_type = AnimationType::Jump;
			Event* piston_event = static_cast<Event*>(obj->GetComponent(ComponentType::Event));
			std::cout << "piston event linked event:";
			std::cout << piston_event->linked_event << std::endl;

			//  Change the animation of door and disable the body of door
			for (size_t j = 0; j < objectFactory->NumberOfObjects(); j++) {
				Object* obj2 = objectFactory->getObjectWithID((long)j);
				if (obj2->GetName() == "door") {
					Event* door_event = static_cast<Event*>(obj2->GetComponent(ComponentType::Event));
					if (piston_event->linked_event == door_event->linked_event) {
						audio->playSfx(AudioType::Sliding_Door_Open);
						Animation* door_animation = static_cast<Animation*>(obj2->GetComponent(ComponentType::Animation));
						door_animation->fixed = true;
						door_animation->current_type = AnimationType::Jump;
						Body* door_body = static_cast<Body*>(obj2->GetComponent(ComponentType::Body));
						door_body->active = false;
					}
				}
			}
		}
	}
}

void Piston::Shutdown(Object* obj) {
	std::cout << "Piston Script Shutdown : " << obj->GetName() << std::endl;
}

Piston piston("Piston");