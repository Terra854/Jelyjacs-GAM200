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
#include "components/Animation.h"
#include "components/Event.h"
#include "Core_Engine.h"
#include <input.h>
#include <message.h>
//#include <Movement.h>
#include <Audio.h>
#include <SceneLoader.h>

Object* scale_and_rotate;
Object* playerObj;
Object* MovingPlatform;
Object* dynamic_collision;
bool moving_platform_direction = false;

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


	//LoadScene("Asset/Levels/tutorial_level.json");

	//playerObj = objectFactory->getPlayerObject();
	//MovingPlatform = objectFactory->getObjectWithID(objectFactory->FindObject("elevator")->GetId());

	/*
	Object* testObj;
	Object* floor1;
	Object* floor2;
	Object* floor3;
	//Transform * trans;
	//Texture* texture;

	std::cout << "Background" << std::endl;
	//testObj = objectFactory->createObject("../Asset/Objects/background.json");
	//scale_and_rotate = objectFactory->createObject("../Asset/Objects/scale-and-rotate.json");
	//std::cout << "test Object 2" << std::endl;
	//testObj2 = objectFactory->createObject("../drop-forever.json");

	LoadScene("../Asset/Levels/testscene.json");
	//floor1 = objectFactory->createObject("../Asset/Objects/mapbox.json");
	//floor2 = objectFactory->createObject("../Asset/Objects/mapbox.json");
	floor3 = objectFactory->createObject("../Asset/Objects/mapbox.json");
	Transform* tran_pt = static_cast<Transform*>(floor3->GetComponent(ComponentType::Transform)); // change to different obj
	tran_pt->Position.x = 210;
	Rectangular* rect_pt = static_cast<Rectangular*>(floor3->GetComponent(ComponentType::Body));
	rect_pt->Initialize();

	// Test cloning
	Object* testclone;
	testclone = objectFactory->cloneObject(floor3);
	tran_pt = static_cast<Transform*>(testclone->GetComponent(ComponentType::Transform)); // change to different obj
	tran_pt->Position.x = -160;
	tran_pt->Position.y = -250;
	rect_pt = static_cast<Rectangular*>(testclone->GetComponent(ComponentType::Body));
	rect_pt->Initialize();

	//Transform* tran_pt = static_cast<Transform*>((objectFactory->getObjectWithID(2))->GetComponent(ComponentType::Transform));

	// offset objects
	//tran_pt->Position.x = 190;

	// @Sen Chuan if you move position like that, remember to call these 2 functions cause collision
	// box still using the old position
	//Rectangular* rect_pt = static_cast<Rectangular*>((objectFactory->getObjectWithID(2))->GetComponent(ComponentType::Body)); // @Sen Chuan
	//rect_pt->Initialize(); // @Sen Chuan

	//tran_pt = static_cast<Transform*>((objectFactory->getObjectWithID(3))->GetComponent(ComponentType::Transform)); // change to different obj
	//tran_pt->Position.x = 380;
	//tran_pt->Position.y = -300;

	//rect_pt = static_cast<Rectangular*>((objectFactory->getObjectWithID(3))->GetComponent(ComponentType::Body));
	//rect_pt->Initialize();

	// Floor
	for (int i = 0; i < 44; i++) {
		Object* floor = objectFactory->createObject("../Asset/Objects/mapbox.json");
		Transform* floor_t = static_cast<Transform*>(floor->GetComponent(ComponentType::Transform));
		floor_t->Position = { -1100.0f + (static_cast<Transform*>(floor->GetComponent(ComponentType::Transform))->Scale_x * (float)i), -500.0f };
		Rectangular* floor_b = static_cast<Rectangular*>(floor->GetComponent(ComponentType::Body)); // @Sen Chuan
		floor_b->Initialize();
	}

	// Dynamic collision
	dynamic_collision = objectFactory->createObject("../Asset/Objects/mapbox.json");
	Transform* dynamic_collision_t = static_cast<Transform*>(dynamic_collision->GetComponent(ComponentType::Transform));
	dynamic_collision_t->Position = { -1000.0f , -446.0f };
	Rectangular* dynamic_collision_b = static_cast<Rectangular*>(dynamic_collision->GetComponent(ComponentType::Body));
	dynamic_collision_b->Initialize();
	Physics* player_physics = new Physics();
	dynamic_collision->AddComponent(player_physics);

	//std::cout << "test bottom_line" << std::endl;
	//bottom_line = objectFactory->createObject("../bottom_line.json");
	//trans = static_cast<Transform*>( testObj->GetComponent(ComponentType::Transform));

	//alternate way to get component without cast
	//trans = testObj->GetComponent_NoCast<Transform>(ComponentTypeId::CT_Transform);
	//trans->Mass = 0.5f;
	//Getting a Object pointer with the game object ID
	//testObj = objectFactory->getObjectWithID(0);
	//texture = static_cast<Texture*>(testObj->GetComponent(ComponentType::Texture));
	std::cout << "test Player" << std::endl;
	playerObj = objectFactory->createObject("../Asset/Objects/player.json");
	*/
}

/******************************************************************************
* Update
* - Update Logic when there is user input
*******************************************************************************/
void GameLogic::Update() {

	/*
	for (auto& it : behaviourComponents)
	{
		behaviours[it->GetBehaviourIndex()]->updateBehaviour(it->GetOwner);
	}
	*/
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

	if (input::IsPressed(KEY::c)) {
		if (camera2D->scale.x == 1.0f || camera2D->scale.y == 1.0f) {
			camera2D->scale = { 2.0f, 2.0f };
		}else camera2D->scale = { 1.0f, 1.0f };
	}


	// Movement for Player
	// If button Pressed, changed velocity
	playerObj = objectFactory->getPlayerObject();

	if (playerObj != nullptr) {
		Physics* player_physics = static_cast<Physics*>(playerObj->GetComponent(ComponentType::Physics));
		Animation* player_animation = static_cast<Animation*>(playerObj->GetComponent(ComponentType::Animation));
		player_physics->Velocity.x = 0.0f;
		if (player_animation->face_right)player_animation->current_type = AnimationType::Idle;
		else player_animation->current_type = AnimationType::Idle_left;
		bool moving = false;
		if (input::IsPressed(KEY::w)) {
			MovementKey msg(up);
			engine->Broadcast(&msg);
			//if (static_cast<Rectangular*>(playerObj->GetComponent(ComponentType::Body))->collision_flag & COLLISION_BOTTOM) {
			if (player_physics->Velocity.y == 0.0f) {
				player_physics->Velocity.y = 1000.0f;
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


		// Let the player loop around the window
		/*
		Transform* t = static_cast<Transform*>(playerObj->GetComponent(ComponentType::Transform));
		t->Position.x = t->Position.x > 1000.0f ? -1000.0f : t->Position.x;
		t->Position.x = t->Position.x < -1000.0f ? 1000.0f : t->Position.x;
		*/
		// Audio for Character Movement
		if ((player_physics->Velocity.y == 0.f) && moving) {
			audio->startWalking();
		}
		else {
			audio->stopWalking();
			moving = false;
		}

		// Printing Player Position by pressing Z
		/*
		if (input::IsPressed(KEY::z)) {
			Transform* player_t = static_cast<Transform*>(playerObj->GetComponent(ComponentType::Transform));
			std::cout << "Printing player Position : " << player_t->Position.x << ", " << player_t->Position.y << std::endl;
		}
		*/
		for (size_t i = 0; i < objectFactory->NumberOfObjects(); i++) {
			Object* obj = objectFactory->getObjectWithID(i);

			if (obj == nullptr)
				continue;

			if (obj->GetName() == "piston") {
				Transform* piston_t = static_cast<Transform*>(obj->GetComponent(ComponentType::Transform));
				Rectangular* piston_b = static_cast<Rectangular*>(obj->GetComponent(ComponentType::Body));
				//Transform* player_t = static_cast<Transform*>(playerObj->GetComponent(ComponentType::Transform));

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
						Object* obj2 = objectFactory->getObjectWithID(j);
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
				//bool moving_platform_direction;

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
	//Movement for Moving Platform
	/*
	if (objectFactory->FindObject("elevator") != nullptr)
		MovingPlatform = objectFactory->getObjectWithID(objectFactory->FindObject("elevator")->GetId());

	if (MovingPlatform != nullptr) {
		Physics* moving_platform_physics = static_cast<Physics*>(MovingPlatform->GetComponent(ComponentType::Physics));
		Transform* moving_platform_t = static_cast<Transform*>(MovingPlatform->GetComponent(ComponentType::Transform));
		moving_platform_physics->Velocity.x = 0.0f;
		float moving_platform_speed;
		//bool moving_platform_direction;

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
	*/
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