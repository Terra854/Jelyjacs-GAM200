/* !
@file GameLogic.cpp
@author Luke Goh
@date	28/9/2023

This file contains the definitions of the functions that are part of the Game Logic system
*//*__________________________________________________________________________*/
#include <Debug.h>
#include "GameLogic.h"
#include <iostream>
//#include "Assets Manager/file_reader.h"
#include "Factory.h"
#include "ComponentCreator.h"
#include "components/Transform.h"
#include "components/Texture.h"
#include "components/Body.h"
#include "components/Physics.h"
#include "components/PlayerControllable.h"
#include "Core_Engine.h"
#include <input.h>
#include <message.h>
//#include <Movement.h>
#include <Audio.h>
#include <GameStateManager.h>

//Object* scale_and_rotate;
Object* playerObj;
//Object* dynamic_collision;

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

	(*fpInitialize)();
	std::cout << "test Player" << std::endl;
	playerObj = objectFactory->createObject("../Asset/Objects/player.json");
	
}

/******************************************************************************
* Update
* - Update Logic when there is user input
*******************************************************************************/
void GameLogic::Update() {
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
	// 
	Physics* p = static_cast<Physics*>(playerObj->GetComponent(ComponentType::Physics));
	p->Velocity.x = 0.0f;
	bool moving = false;
	if (input::IsPressed(KEY::w)) {
		MovementKey msg(up);
		engine->Broadcast(&msg);
		//if (static_cast<Rectangular*>(playerObj->GetComponent(ComponentType::Body))->collision_flag & COLLISION_BOTTOM) {
		if (p->Velocity.y == 0.0f) {
			p->Velocity.y = 1500.0f;
			audio->playJump();
		}
	}
	if (input::IsPressedRepeatedly(KEY::a)) {
		MovementKey msg(left);
		engine->Broadcast(&msg);
		p->Velocity.x -= 500.0f;
		moving = true;
	}
	if (input::IsPressedRepeatedly(KEY::d)) {
		MovementKey msg(right);
		engine->Broadcast(&msg);
		p->Velocity.x += 500.0f;
		moving = true;
	}

	// Let the player loop around the window
	Transform* t = static_cast<Transform*>(playerObj->GetComponent(ComponentType::Transform));
	t->Position.x = t->Position.x > 1000.0f ? -1000.0f : t->Position.x;
	t->Position.x = t->Position.x < -1000.0f ? 1000.0f : t->Position.x;

	// Audio for Character Movement
	if ((p->Velocity.y == 0.f) && moving) {
		audio->startWalking();
	}
	else {
		audio->stopWalking();
		moving = false;
	}

	// Printing Player Position by pressing Z
	if (input::IsPressed(KEY::z)) {
		Transform* player_t = static_cast<Transform*>(playerObj->GetComponent(ComponentType::Transform));
		std::cout << "Printing player Position : " << player_t->Position.x << ", " << player_t->Position.y << std::endl;
	}

	(*fpUpdate)();
}