/* !
@file GameLogic.cpp
@author Luke Goh
@date	28/9/2023

This file contains the definitions of the functions that are part of the Game Logic system
*//*__________________________________________________________________________*/
#include <Debug.h>
#include "GameLogic.h"
#include <iostream>
#include "Factory.h"
#include "ComponentCreator.h"
#include "components/Transform.h"
#include "components/Texture.h"
#include "components/Body.h"
#include "components/Physics.h"
#include "components/PlayerControllable.h"
#include "components/Animation.h"
#include "components/Event.h"
#include "Core_Engine.h"
#include <input.h>
#include <message.h>
#include <Audio.h>
#include <Scenes.h>
#include <PhysicsSystem.h>

GameLogic* Logic = NULL;
Object* scale_and_rotate;
Object* playerObj;
Object* MovingPlatform;
Object* dynamic_collision;
bool moving_platform_direction = false;

GameLogic::GameLogic() {
	Logic = this;
	
	playerObj = nullptr;
	MovingPlatform = nullptr;
	dynamic_collision = nullptr;
	moving_platform_direction = false;
}

GameLogic::~GameLogic() {
	
}
/******************************************************************************
* MessageRelay
* - Get Message and React Accordingly
* - For Movement Keys Debug
*******************************************************************************/
void GameLogic::MessageRelay(Message_Handler* msg) {
	// For Movement Key Display
	if (msg->GetMessage() == MessageID::Movement) {
		MovementKey* temp = static_cast<MovementKey*>(msg);
		std::cout << temp->dir << std::endl;
	}
}
/******************************************************************************
* Initialize
* - Initialize Objects and their Components
*******************************************************************************/
void GameLogic::Initialize()
{
	// In order to use the game object factory, we need to register the components we want to use first like this
	// When we create new types of components, we need to add it in there as well
	objectFactory->AddComponentCreator("Transform", new ComponentCreator<Transform>());
	objectFactory->AddComponentCreator("Texture", new ComponentCreator<Texture>());
	objectFactory->AddComponentCreator("Rectangle", new ComponentCreator<Rectangular>());
	objectFactory->AddComponentCreator("Circle", new ComponentCreator<Circular>());
	objectFactory->AddComponentCreator("Line", new ComponentCreator<Lines>());
	objectFactory->AddComponentCreator("Physics", new ComponentCreator<Physics>());
	objectFactory->AddComponentCreator("Player", new ComponentCreator<PlayerControllable>());
	objectFactory->AddComponentCreator("Event", new ComponentCreator<Event>());
	objectFactory->AddComponentCreator("Behaviour", new ComponentCreator<Behaviour>());

	//for(auto const)

	//LoadScene("Asset/Levels/testsave.json");

	LoadScene("Asset/Levels/tutorial_level.json");
	SaveScene("Asset/Levels/testsave.json");
}

/******************************************************************************
* Update
* - Update Logic when there is user input
*******************************************************************************/
void GameLogic::Update() {

	// Do not update if the game is paused
	if (engine->isPaused())
		return;
	int x = 0;

	for (auto& behaviour : behaviours) {
		behaviour.second->Update(b_objects[x]);
		std::cout << "Behaviour " << x << " updated" << std::endl;
		x++;
	}
	
	// If Left Click, show mouse position
	if (input::IsPressed(KEY::mouseL)) {
		std::cout << "Mouse Position is :  X = " << input::GetMouseX() << ", Y = " << input::GetMouseY() << std::endl;
		Message_Handler msg(MessageID::Event_Type::MouseClick);
		engine->Broadcast(&msg);
	}

	// If press esc button, quit the game
	if (input::IsPressed(KEY::esc)) {
		Message_Handler msg(MessageID::Event_Type::Quit);
		engine->Broadcast(&msg);
	}


	// Movement for Player
	// If button Pressed, changed velocity
	playerObj = objectFactory->getPlayerObject();

	if (playerObj != nullptr && playerObj->GetComponent(ComponentType::Physics) != nullptr) {
		Physics* player_physics = static_cast<Physics*>(playerObj->GetComponent(ComponentType::Physics));
		Animation* player_animation = static_cast<Animation*>(playerObj->GetComponent(ComponentType::Animation));
		player_physics->Velocity.x = 0.0f;
		if (player_animation->face_right)player_animation->current_type = AnimationType::Idle;
		else player_animation->current_type = AnimationType::Idle_left;
		bool moving = false;
		if (input::IsPressed(KEY::w)) {
			MovementKey msg(up);
			engine->Broadcast(&msg);
			if (player_physics->Velocity.y == 0.0f) {
				player_physics->Force = 60000.0f + gravity;
				audio->playJump();
			}
		}
		if (input::IsPressedRepeatedly(KEY::a)) {
			MovementKey msg(left);
			engine->Broadcast(&msg);
			player_physics->Velocity.x -= 500.0f;
			moving = true;
			player_animation->face_right = false;
			player_animation->current_type = AnimationType::Run_left;
		}
		if (input::IsPressedRepeatedly(KEY::d)) {
			MovementKey msg(right);
			engine->Broadcast(&msg);
			player_physics->Velocity.x += 500.0f;
			moving = true;
			player_animation->face_right = true;
			player_animation->current_type = AnimationType::Run;
		}

		if (player_physics->Velocity.y != 0.0f) {
			player_animation->jump_fixed = true;

			if (player_animation->face_right)player_animation->current_type = AnimationType::Jump;
			else player_animation->current_type = AnimationType::Jump_left;
		}
		else player_animation->jump_fixed = false;

		// Audio for Character Movement
		if ((player_physics->Velocity.y == 0.f) && moving) {
			audio->startWalking();
		}
		else {
			audio->stopWalking();
			moving = false;
		}

		for (size_t i = 0; i < objectFactory->NumberOfObjects(); i++) {
			Object* obj = objectFactory->getObjectWithID((long)i);

			if (obj == nullptr)
				continue;

			if (obj->GetName() == "piston") {
				Rectangular* piston_b = static_cast<Rectangular*>(obj->GetComponent(ComponentType::Body));

				if (piston_b->collision_flag & COLLISION_TOP) {
					Animation* piston_animation = static_cast<Animation*>(obj->GetComponent(ComponentType::Animation));
					piston_animation->fixed = true;
					if (piston_animation->current_type == AnimationType::Jump)continue;
					piston_animation->current_type = AnimationType::Jump;
					Event* piston_event = static_cast<Event*>(obj->GetComponent(ComponentType::Event));
					std::cout << "pisotn event linked event:";
					std::cout << piston_event->linked_event << std::endl;
					//check the door
					for (size_t j = 0; j < objectFactory->NumberOfObjects(); j++) {
						Object* obj2 = objectFactory->getObjectWithID((long)j);
						if (obj2->GetName() == "door") {
							Event* door_event = static_cast<Event*>(obj2->GetComponent(ComponentType::Event));
							if (piston_event->linked_event == door_event->linked_event) {
								audio->playSlidingDoor();
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
			else if (obj->GetName() == "elevator") {
				MovingPlatform = objectFactory->getObjectWithID(objectFactory->FindObject("elevator")->GetId());

				Physics* moving_platform_physics = static_cast<Physics*>(MovingPlatform->GetComponent(ComponentType::Physics));
				Transform* moving_platform_t = static_cast<Transform*>(MovingPlatform->GetComponent(ComponentType::Transform));
				moving_platform_physics->Velocity.x = 0.0f;
				float moving_platform_speed;

				// if the platform reach the max/min height, change direction
				if (moving_platform_t->Position.y >= 160.0f) { // 160 is the max height of the platform
					moving_platform_direction = true;
				}
				if (moving_platform_t->Position.y <= -160.0f) { // -160 is the min height of the platform
					moving_platform_direction = false;
				}
				moving_platform_speed = moving_platform_direction ? -70.0f : 70.0f;
				moving_platform_physics->Velocity.y = moving_platform_speed;

				if (input::IsPressed(KEY::z)) {
					std::cout << "Moving Platform Position : " << moving_platform_t->Position.x << ", " << moving_platform_t->Position.y << std::endl;
				}
			}
		}
	}
}

void GameLogic::AddBehaviour(std::string name, LogicScript* behaviour)
{
	behaviours[name] = behaviour;
}

void GameLogic::AddObject(Object* obj)
{
	b_objects.push_back(obj);
}
