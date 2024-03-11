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

#include <Audio.h>
#include <Scenes.h>
#include <PhysicsSystem.h>
#include <Scripts/Spark.h>
#include <SceneManager.h>


std::map<std::string, LogicScript*> LogicScript::temp_scriptmap;
GameLogic* Logic = NULL;
Object* scale_and_rotate;
Object* GameLogic::playerObj;
Object* dynamic_collision;
int CatPower = 0;
bool GameLogic::restarting;

bool one_time = false;
bool cheat = false;

GameLogic::GameLogic() {
	Logic = this;
	GameLogic::playerObj = nullptr;
	dynamic_collision = nullptr;
	GameLogic::restarting = false;
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
		/*
		MovementKey* temp = static_cast<MovementKey*>(msg);
		switch (temp->dir) {
			case up:
				std::cout << "UP" << std::endl;
				break;
			case down:
				std::cout << "DOWN" << std::endl;
				break;
			case left:
				std::cout << "LEFT" << std::endl;
				break;
			case right:
				std::cout << "RIGHT" << std::endl;
				break;
			default:
				break;
		}
		*/
	}
}
/******************************************************************************
* Initialize
* - Initialize Objects and their Components
*******************************************************************************/
void GameLogic::Initialize()
{
	//LoadSceneFromJson("Asset/Levels/testsave.json");
	//SaveScene("Asset/Levels/testsave.json");

	// Clear all behaviours from the container
	behaviours.clear();

	// Load all behaviours into the container
	LoadScripts();
	for (auto iter : behaviourComponents ) {
		
		if (behaviours[iter->GetBehaviourName()] == nullptr) {
			std::cout << "Behaviour " << iter->GetBehaviourName() << " is null" << std::endl;
			continue;
		}
		else {
			behaviours[iter->GetBehaviourName()]->Start(iter->GetOwner());
			std::cout << "Owner ID: " << iter->GetOwner()->GetId() << std::endl;
		}
	}
	cheat = false;
	CatPower = 0;
	std::cout << "GameLogic Initialized" << std::endl;
	std::cout << "Number of Behaviour Components: " << behaviourComponents.size() << std::endl;
	std::cout << "Number of Behaviour Scripts: " << behaviours.size() << std::endl;

	GameLogic::playerObj = objectFactory->getPlayerObject();
	//std::cout << "Player's behaviour is " << static_cast<Behaviour*>(GameLogic::playerObj->GetComponent(ComponentType::Behaviour))->GetBehaviourName() << " || " << static_cast<Behaviour*>(GameLogic::playerObj->GetComponent(ComponentType::Behaviour))->GetBehaviourIndex() << std::endl;
}

/******************************************************************************
* Update
* - Update Logic 
*******************************************************************************/
void GameLogic::Update() {

	// Do not update if the game is paused and the level editor is active
	if (engine->isPaused() && engine->debug_gui_active)
		return;

	// Update all behaviours
	int counter = 0;
	for (auto& iter : behaviourComponents) {
		//Check if the object is the player
		if (GameLogic::playerObj != nullptr && GameLogic::playerObj->GetComponent(ComponentType::Behaviour) != nullptr) {
			if(iter->GetBehaviourName() != "NULL")
				if (iter->GetBehaviourName() == static_cast<Behaviour*>(GameLogic::playerObj->GetComponent(ComponentType::Behaviour))->GetBehaviourName()) {
					behaviours[iter->GetBehaviourName()]->Update(GameLogic::playerObj);
					counter++;
					continue;
				}
		}
		// Update all other objects
		for (auto it : objectFactory->FindAllObjectsByName(iter->GetOwner()->GetName())) {
			if (iter->GetBehaviourName() == "NULL") {
				continue;
			}
			if (!engine->isPaused() || behaviours[iter->GetBehaviourName()]->executeOnPause)
			{
				//std::cout << "Running behaviour: " << iter->GetBehaviourName() << std::endl;
				behaviours[iter->GetBehaviourName()]->Update(it);
			}
			counter++;
		}
	}
	if (restarting) {
		SceneManager::RestartScene();
		SceneManager::PlayScene();
		GameLogic::restarting = false;
		return;
	}
	/********************************************************************************
	*
	*	Basic Input Handling Logic
	*
	*********************************************************************************/
	
	// If press esc button, pause the game
	if (input::IsPressed(KEY::esc)) {
		//Message_Handler msg(MessageID::Event_Type::Quit);
		//engine->Broadcast(&msg);
		engine->setPause();
	}
	
	// Only proceed if Finn and Spark are inside
	if (GameLogic::playerObj != nullptr) {
		if (input::IsPressed(KEY::e)) {
			if (GameLogic::playerObj->GetName() == "Finn") {
				Object* temp = objectFactory->FindObject("Spark");
				GameLogic::playerObj = temp == nullptr ? GameLogic::playerObj : objectFactory->FindObject("Spark");
				Spark::Just_detached = true;
				Spark::Connected_to_Finn = false;
				static_cast<Body*>(temp->GetComponent(ComponentType::Body))->active = true;
				std::cout << "Switched to Spark" << std::endl;
			}
			else if (GameLogic::playerObj->GetName() == "Spark") {
				Object* temp = objectFactory->FindObject("Finn");
				GameLogic::playerObj = temp == nullptr ? GameLogic::playerObj : objectFactory->FindObject("Finn");
				std::cout << "Switched to Finn" << std::endl;
			}
		}

		for (size_t i = 0; i < objectFactory->NumberOfObjects(); i++) {
			Object* obj = objectFactory->getObjectWithID((long)i);

			if (obj == nullptr)
				continue;
		}

		/*****************************************************************************************
		*
		*		Cheat Codes
		*
		******************************************************************************************/
		if (input::IsPressed(KEY::one)) {
			if (!cheat) {
				std::cout << "Cheat Mode Activated" << std::endl;
				cheat = true;
			}
			else {
				std::cout << "Cheat Mode Deactivated" << std::endl;
				cheat = false;

			}
		}
		if (input::IsPressed(KEY::two)) {
			if (objectFactory->FindObject("MainDoor") != nullptr && objectFactory->FindObject("Finn") != nullptr && objectFactory->FindObject("Spark") != nullptr) {
				static_cast<Transform*>(objectFactory->FindObject("Finn")->GetComponent(ComponentType::Transform))->Position = static_cast<Transform*>(objectFactory->FindObject("MainDoor")->GetComponent(ComponentType::Transform))->Position;
				static_cast<Transform*>(objectFactory->FindObject("Spark")->GetComponent(ComponentType::Transform))->Position = static_cast<Transform*>(objectFactory->FindObject("MainDoor")->GetComponent(ComponentType::Transform))->Position;
			}
		}

		if (cheat) {
			Physics* temp_p = static_cast<Physics*>(GameLogic::playerObj->GetComponent(ComponentType::Physics));
			temp_p->AffectedByGravity = false;
			static_cast<Body*>(playerObj->GetComponent(ComponentType::Body))->active = false;
			if (input::IsPressed(KEY::w)) {
				temp_p->Velocity.y = 300.00f;
			}
			if (input::IsPressed(KEY::s)) {
				temp_p->Velocity.y = -300.0f;
			}
		}
		else {
			static_cast<Physics*>(playerObj->GetComponent(ComponentType::Physics))->AffectedByGravity = true;
			static_cast<Body*>(playerObj->GetComponent(ComponentType::Body))->active = true;
		}
	}
	/*****************************************************************************************
	*
	*	Debugging
	*
	*******************************************************************************************/
	//std::cout << "Number of Behaviour Components: " << behaviourComponents.size() << std::endl;
	// If Left Click, show mouse position
	if (input::IsPressed(KEY::mouseL)) {
		std::cout << "Mouse Position is :  X = " << input::GetMouseX() << ", Y = " << input::GetMouseY() << std::endl;
		Message_Handler msg(MessageID::Event_Type::MouseClick);
		engine->Broadcast(&msg);
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
/******************************************************************************
*
*	AddBehaviour : Add a behaviour to the container
*
*******************************************************************************/
void GameLogic::AddBehaviour(std::string name, LogicScript* behaviour)
{
	behaviours[name] = behaviour;
	std::cout <<  "Added Behaviour to container. Name : "<< name <<"  |  Container Size : " << behaviours.size() << std::endl;
}
/******************************************************************************
*
*	LoadScripts : Load all scripts into the container
*
*******************************************************************************/
void GameLogic::LoadScripts() {
	for (auto& it : LogicScript::temp_scriptmap) {
		this->AddBehaviour(it.first, it.second);
	}
	std::cout << "Loaded Temporary Script Map: " << LogicScript::temp_scriptmap.size() << std::endl;
}

/******************************************************************************
*
*	CheckBehaviour : Check if a behaviour exists in the container
*
*******************************************************************************/
bool GameLogic::CheckBehaviour(std::string name) {
	return behaviours.find(name) == behaviours.end() ? false : true;
}