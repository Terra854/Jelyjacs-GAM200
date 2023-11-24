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
#include "Object.h"
#include "components/Behaviour.h"
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
//#include <Movement.h>
#include <Audio.h>
#include <Scenes.h>
#include <PhysicsSystem.h>


std::map<std::string, LogicScript*> LogicScript::temp_scriptmap;
GameLogic* Logic = NULL;
Object* scale_and_rotate;
Object* GameLogic::playerObj;
Object* MovingPlatform;
Object* dynamic_collision;
bool moving_platform_direction = false;



GameLogic::GameLogic() {
	Logic = this;
	GameLogic::playerObj = nullptr;
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
		//std::cout << temp->dir << std::endl;
	}
}
/******************************************************************************
* Initialize
* - Initialize Objects and their Components
*******************************************************************************/
void GameLogic::Initialize()
{
	//LoadScene("Asset/Levels/testsave.json");
	//SaveScene("Asset/Levels/testsave.json");

	behaviours.clear();

	LoadScripts();
	for (auto iter : behaviourComponents ) {
		
		if (behaviours[iter->GetBehaviourName()] == nullptr) {
			std::cout << "Behaviour " << iter->GetBehaviourName() << " is null" << std::endl;
			continue;
		}
		else 
			behaviours[iter->GetBehaviourName()]->Start(iter->GetOwner());
	}

	std::cout << "GameLogic Initialized" << std::endl;
	std::cout << "Number of Behaviour Components: " << behaviourComponents.size() << std::endl;
	std::cout << "Number of Behaviour Scripts: " << behaviours.size() << std::endl;

	GameLogic::playerObj = objectFactory->getPlayerObject();
	//std::cout << "Player's behaviour is " << static_cast<Behaviour*>(GameLogic::playerObj->GetComponent(ComponentType::Behaviour))->GetBehaviourName() << " || " << static_cast<Behaviour*>(GameLogic::playerObj->GetComponent(ComponentType::Behaviour))->GetBehaviourIndex() << std::endl;
}

/******************************************************************************
* Update
* - Update Logic when there is user input
*******************************************************************************/
void GameLogic::Update() {

	// Do not update if the game is paused
	if (engine->isPaused())
		return;

	for (auto& iter : behaviourComponents) {
		//Check if the object is the player
		if (GameLogic::playerObj != nullptr && GameLogic::playerObj->GetComponent(ComponentType::Behaviour) != nullptr) {
			if (iter->GetBehaviourName() == static_cast<Behaviour*>(GameLogic::playerObj->GetComponent(ComponentType::Behaviour))->GetBehaviourName()) {
				behaviours[iter->GetBehaviourName()]->Update(GameLogic::playerObj);
			}
		}
		else {
			// Update the behaviour
			for (auto it : objectFactory->FindAllObjectsByName(iter->GetOwner()->GetName())) {
				behaviours[iter->GetBehaviourName()]->Update(it);
			}
		}
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
	
	if (GameLogic::playerObj != nullptr) {
		if (input::IsPressed(KEY::e)) {
			if (GameLogic::playerObj->GetName() == "Finn") {
				Object* temp = objectFactory->FindObject("Spark");
				GameLogic::playerObj = temp == nullptr ? GameLogic::playerObj :objectFactory->FindObject("Spark");
				std::cout << "Switched to Spark" << std::endl;
			}
			else if(GameLogic::playerObj->GetName() == "Spark") {
				Object* temp = objectFactory->FindObject("Finn");
				GameLogic::playerObj = temp == nullptr ? GameLogic::playerObj : objectFactory->FindObject("Finn");
				std::cout << "Switched to Finn" << std::endl;
			}
			else {
				GameLogic::playerObj = GameLogic::playerObj;
			}
		}

		/*for (size_t i = 0; i < objectFactory->NumberOfObjects(); i++) {
			Object* obj = objectFactory->getObjectWithID((long)i);

			if (obj == nullptr)
				continue;
		}*/
	}
	/*
	// Rotation of an object
	Transform* t2 = static_cast<Transform*>(scale_and_rotate->GetComponent(ComponentType::Transform));

	if (input::IsPressedRepeatedly(KEY::up)) {
		t2->Scale_x += 1.0f;
		t2->Scale_y += 1.0f;
	}
	if (input::IsPressedRepeatedly(KEY::down)) {
		t2->Scale_x = t2->Scale_x >= 1.0f ? t2->Scale_x - 1.0f : 0.f;
		t2->Scale_y = t2->Scale_y >= 1.0f ? t2->Scale_y - 1.0f : 0.f;
	}
	if (input::IsPressedRepeatedly(KEY::left)) {
		t2->Rotation += 0.01f;
	}
	if (input::IsPressedRepeatedly(KEY::right)) {
		t2->Rotation -= 0.01f;
	}
	*/

	// Dynamic collision
	//Physics* dynamic_collision_p = static_cast<Physics*>(dynamic_collision->GetComponent(ComponentType::Physics));
	//dynamic_collision_p->Velocity.x = 200.0f;
	//Transform* dynamic_collision_t = static_cast<Transform*>(dynamic_collision->GetComponent(ComponentType::Transform));
	//dynamic_collision_t->Position.x = dynamic_collision_t->Position.x < 1000.0f ? dynamic_collision_t->Position.x : -1000.0f;

	/*
	// DEBUG: Print out collision flags
	int c_flag = static_cast<Rectangular*>(playerObj->GetComponent(ComponentType::Body))->collision_flag;
	std::cout << "FLAG: " << c_flag <<
		" LEFT: " << ((c_flag & COLLISION_LEFT) ? "YES" : "NO") <<
		" RIGHT: " << ((c_flag & COLLISION_RIGHT) ? "YES" : "NO") <<
		" TOP: " << ((c_flag & COLLISION_TOP) ? "YES" : "NO") <<
		" BOTTOM: " << ((c_flag & COLLISION_BOTTOM) ? "YES" : "NO") << std::endl;
	std::cout << "#####################################################################" << std::endl;
	*/
}

void GameLogic::AddBehaviour(std::string name, LogicScript* behaviour)
{
	behaviours[name] = behaviour;
	std::cout <<  "Added Behaviour to container. Name : "<< name <<"  |  Container Size : " << behaviours.size() << std::endl;
}

void GameLogic::LoadScripts() {
	for (auto& it : LogicScript::temp_scriptmap) {
		this->AddBehaviour(it.first, it.second);
	}
	std::cout << "Loaded Temporary Script Map: " << LogicScript::temp_scriptmap.size() << std::endl;
}

bool GameLogic::CheckBehaviour(std::string name) {
	return behaviours.find(name) == behaviours.end() ? false : true;
}