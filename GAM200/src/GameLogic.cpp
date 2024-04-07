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
Vec2 start_position;
int CatPower = 0;
bool teleporting = false;
float death_timer = 0.f;
bool GameLogic::death;
bool GameLogic::restarting;
bool GameLogic::no_movement;

bool one_time = false;
bool cheat = false;

GameLogic::GameLogic() {
	Logic = this;
	GameLogic::playerObj = nullptr;
	dynamic_collision = nullptr;
	GameLogic::restarting = false;
	GameLogic::death = false;
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
		GameLogic::no_movement = false;
	}
	else if (msg->GetMessage() == MessageID::NoMovement)
	{
		GameLogic::no_movement = true;
	}
}
/******************************************************************************
* Initialize
* - Initialize Objects and their Components
*******************************************************************************/
void GameLogic::Initialize()
{
	// Clear all behaviours from the container
	behaviours.clear();

	// Load all behaviours into the container
	LoadScripts();
	for (auto iter : behaviourComponents ) {
		if (iter->GetBehaviourName().empty())
			continue;


		if (behaviours.at(iter->GetBehaviourName()) == nullptr) {
			std::cout << "Behaviour " << iter->GetBehaviourName() << " is null" << std::endl;
			continue;
		}
		else {
			behaviours.at(iter->GetBehaviourName())->Start(iter->GetOwner());
			std::cout << "Owner ID: " << iter->GetOwner()->GetId() << std::endl;
		}
	}
	cheat = false;
	CatPower = 0;
	std::cout << "GameLogic Initialized" << std::endl;
	std::cout << "Number of Behaviour Components: " << behaviourComponents.size() << std::endl;
	std::cout << "Number of Behaviour Scripts: " << behaviours.size() << std::endl;

	GameLogic::playerObj = objectFactory->getPlayerObject();
	if (playerObj != NULL) {
		start_position = static_cast<Transform*>(playerObj->GetComponent(ComponentType::Transform))->Position;
		if (start_position == Vec2(0.f, 0.f)) {
			std::cout << "Not Working" << std::endl;
			return;
		}
	}
}

/******************************************************************************
* Update
* - Update Logic 
*******************************************************************************/
void GameLogic::Update() {

	// Do not update if the game is paused and the level editor is active
	if (engine->isPaused() && engine->debug_gui_active)
		return;

	button_hover = false;

	// Update all behaviours

	for (int i = 0; i < objectFactory->GetNextId(); i++) {
		
		Object* o = objectFactory->getObjectWithID(i);
		Behaviour* b = static_cast<Behaviour*>(o->GetComponent(ComponentType::Behaviour));

		if (!b) {
			continue;
		}
		if (!engine->isPaused() || behaviours[b->GetBehaviourName()]->executeOnPause)
		{
			behaviours[b->GetBehaviourName()]->Update(o);
		}
	}
	if (death) {
		static Vec2 initialCoord;
		if (death_timer == 0.f) {
			audio->stopBackground();
			audio->playSfx("game_over_hit", 3.f);
			Physics* p = static_cast<Physics*>(playerObj->GetComponent(ComponentType::Physics));
			if (p) {
				p->Velocity.x = p->Velocity.y = 0.f;
				p->AffectedByGravity = false;
			}
			initialCoord = static_cast<Transform*>(playerObj->GetComponent(ComponentType::Transform))->Position;

		}
		death_timer += engine->GetDt();

		float x = (rand() % 1000) / 100.0f;
		float y = (rand() % 1000) / 100.0f;
		Vec2 random_num = { x, y};
		random_num = rand() % 10 > 5 ? random_num : -random_num;

		static_cast<Transform*>(playerObj->GetComponent(ComponentType::Transform))->Position = initialCoord + random_num;

		std::cout << "Death Timer: " << death_timer << std::endl;
		if (death_timer > 1.f) {
			Transform* player_t = static_cast<Transform*>(playerObj->GetComponent(ComponentType::Transform));
			camera2D->TranslateCamera(player_t->Position, start_position, 1.0f);
			restarting = true;
			death = false;
			death_timer = 0.f;
		}
	
	}

	// Button hovered sfx
	if (button_hover && !button_hover_played) {
		audio->playSfx("menu_hoover_over_sfx");
		button_hover_played = true;
	}
	else if (!button_hover){
		button_hover_played = false;
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

		if (objectFactory->GetLayer("PauseMenu") && objectFactory->GetLayer("GameMenu") && objectFactory->GetLayer("HowToPlayMenu")) {
			
			engine->isPaused() ? SceneManager::PlayScene() : SceneManager::PauseScene();

			objectFactory->GetLayer("PauseMenu")->second.first.isVisible = engine->isPaused();
			objectFactory->GetLayer("GameMenu")->second.first.isVisible = !engine->isPaused();
			objectFactory->GetLayer("HowToPlayMenu")->second.first.isVisible = false;
		}
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
			else if (GameLogic::playerObj->GetName() == "Spark" && !teleporting) {
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
			if (!death)
				static_cast<Physics*>(playerObj->GetComponent(ComponentType::Physics))->AffectedByGravity = true;
			
			static_cast<Body*>(playerObj->GetComponent(ComponentType::Body))->active = true;
		}
	}
	/*****************************************************************************************
	*
	*	Debugging
	*
	*******************************************************************************************/
	// If Left Click, show mouse position
	if (input::IsPressed(KEY::mouseL)) {
		std::cout << "Mouse Position is :  X = " << input::GetMouseX() << ", Y = " << input::GetMouseY() << std::endl;
		Message_Handler msg(MessageID::Event_Type::MouseClick);
		engine->Broadcast(&msg);
	}
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