#include <Debug.h>
#include "GameLogic.h"
#include <iostream>
#include "Assets Manager/file_reader.h"
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

Object* scale_and_rotate;
Object* playerObj;
Object* dynamic_collision;

void GameLogic::MessageRelay(Message* msg) {
	// For Movement Key Display
	/*
	if (msg->messageId == MessageID::Movement) {
		MovementKey* temp = static_cast<MovementKey*>(msg);
		std::cout << temp->dir << std::endl;
	}
	*/
}
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

	Object* testObj;
	Object* floor1;
	Object* floor2;
	Object* floor3;
	//Transform * trans;
	//Texture* texture;

	std::cout << "Background" << std::endl;
	testObj = objectFactory->createObject("../background.json");
	scale_and_rotate = objectFactory->createObject("../scale-and-rotate.json");
	//std::cout << "test Object 2" << std::endl;
	//testObj2 = objectFactory->createObject("../drop-forever.json");
	floor1 = objectFactory->createObject("../mapbox.json");
	floor2 = objectFactory->createObject("../mapbox.json");
	floor3 = objectFactory->createObject("../mapbox.json");
	Transform* tran_pt = static_cast<Transform*>((objectFactory->getObjectWithID(2))->GetComponent(ComponentType::Transform));
	tran_pt->Position.x = 190; // offset objectx
	// @Sen Chuan if you move position like that, remember to call these 2 functions cause collision
	// box still using the old position
	Rectangular* rect_pt = static_cast<Rectangular*>((objectFactory->getObjectWithID(2))->GetComponent(ComponentType::Body)); // @Sen Chuan
	rect_pt->Initialize(); // @Sen Chuan

	tran_pt = static_cast<Transform*>((objectFactory->getObjectWithID(3))->GetComponent(ComponentType::Transform)); // change to different obj
	tran_pt->Position.x = 380;
	tran_pt->Position.y = -300;

	rect_pt = static_cast<Rectangular*>((objectFactory->getObjectWithID(3))->GetComponent(ComponentType::Body));
	rect_pt->Initialize();

	std::cout << "test Player" << std::endl;
	playerObj = objectFactory->createObject("../player.json");

	// Floor
	for (int i = 0; i < 40; i++) {
		Object* floor = objectFactory->createObject("../mapbox.json");
		Transform* floor_t = static_cast<Transform*>(floor->GetComponent(ComponentType::Transform));
		floor_t->Position = { -1000.0f + (static_cast<Transform*>(floor->GetComponent(ComponentType::Transform))->Scale_x * (float)i), -500.0f };
		Rectangular* floor_b = static_cast<Rectangular*>(floor->GetComponent(ComponentType::Body)); // @Sen Chuan
		floor_b->Initialize();
	}

	// Dynamic collision
	dynamic_collision = objectFactory->createObject("../mapbox.json");
	Transform* dynamic_collision_t = static_cast<Transform*>(dynamic_collision->GetComponent(ComponentType::Transform));
	dynamic_collision_t->Position = { -1000.0f , -446.0f };
	Rectangular* dynamic_collision_b = static_cast<Rectangular*>(dynamic_collision->GetComponent(ComponentType::Body));
	dynamic_collision_b->Initialize();
	Physics* p = new Physics();
	dynamic_collision->AddComponent(p);

	//std::cout << "test bottom_line" << std::endl;
	//bottom_line = objectFactory->createObject("../bottom_line.json");
	//trans = static_cast<Transform*>( testObj->GetComponent(ComponentType::Transform));

	//alternate way to get component without cast
	//trans = testObj->GetComponent_NoCast<Transform>(ComponentTypeId::CT_Transform);
	//trans->Mass = 0.5f;
	//Getting a Object pointer with the game object ID
	//testObj = objectFactory->getObjectWithID(0);
	//texture = static_cast<Texture*>(testObj->GetComponent(ComponentType::Texture));

	//Broken code. DO NOT REMOVE YET - Jonathan
	/*
	gameObjFactory->AddComponentCreator("Transform", new ComponentCreatorType<Transform>(ComponentTypeId::CT_Transform));

	ComponentCreatorType<Transform> transformComponetCreator(ComponentTypeId::CT_Transform);
	ComponentCreator* transformPtr = transformComponetCreator;

	objFactory gameObjFactory;
	std::string transform = "Transform";

	gameObjFactory.AddComponentCreator(transform, transformPtr);
	*/
}

void GameLogic::Update(float time) {

	// If press esc button, quit the game
	if (input::IsPressed(KEY::esc)) {
		Message msg(MessageID::MessageIDType::Quit);
		engine->Broadcast(&msg);
	}
	//Movement for Player
	// If button Pressed, changed velocity
	Physics* p = static_cast<Physics*>(playerObj->GetComponent(ComponentType::Physics));
	p->Velocity.x = 0.0f;
	bool moving = false;
	if (input::IsPressed(KEY::w)) {
		MovementKey msg(up);
		engine->Broadcast(&msg);

		//if (static_cast<Rectangular*>(playerObj->GetComponent(ComponentType::Body))->collision_flag & COLLISION_BOTTOM) {
		if (p->Velocity.y == 0.0f) {
			p->Velocity.y = 2500.0f;
			audio->playJump();
		}
	}
	if (input::IsPressedRepeatedly(KEY::a)) {
		MovementKey msg(left);
		engine->Broadcast(&msg);

		p->Velocity.x -= 500.0f;
		//std::cout << "Current player position: x=" << t->Position.x << ", y=" << t->Position.y << std::endl;
		moving = true;
	}
	/*
	if (input::IsPressedRepeatedly(KEY::s)) {
		MovementKey msg(down);
		engine->Broadcast(&msg);
	}*/
	if (input::IsPressedRepeatedly(KEY::d)) {
		MovementKey msg(right);
		engine->Broadcast(&msg);

		p->Velocity.x += 500.0f;
		//std::cout << "Current player position: x=" << t->Position.x << ", y=" << t->Position.y << std::endl;
		moving = true;
	}

	if ((p->Velocity.y == 0.f) && moving) {
		audio->startWalking();
	}
	else {
		audio->stopWalking();
		moving = false;
	}
	// Printing Player Position by pressing s
	if (input::IsPressed(KEY::p)) {
		Transform* player_t = static_cast<Transform*>(playerObj->GetComponent(ComponentType::Transform));
		std::cout << "Printing player Position : " << player_t->Position.x << ", " << player_t->Position.y << std::endl;
	}

	// Rotation of an object
	Transform* t = static_cast<Transform*>(scale_and_rotate->GetComponent(ComponentType::Transform));

	if (input::IsPressedRepeatedly(KEY::up)) {
		t->Scale_x += 1.0f;
		t->Scale_y += 1.0f;
	}
	if (input::IsPressedRepeatedly(KEY::down)) {
		t->Scale_x = t->Scale_x >= 1.0f ? t->Scale_x - 1.0f : 0.f;
		t->Scale_y = t->Scale_y >= 1.0f ? t->Scale_y - 1.0f : 0.f;
	}
	if (input::IsPressedRepeatedly(KEY::left)) {
		t->Rotation += 0.01f;
	}
	if (input::IsPressedRepeatedly(KEY::right)) {
		t->Rotation -= 0.01f;
	}

	// Dynamic collision

	Physics* dynamic_collision_p = static_cast<Physics*>(dynamic_collision->GetComponent(ComponentType::Physics));
	dynamic_collision_p->Velocity.x = 200.0f;
	Transform* dynamic_collision_t = static_cast<Transform*>(dynamic_collision->GetComponent(ComponentType::Transform));
	dynamic_collision_t->Position.x = dynamic_collision_t->Position.x < 1000.0f ? dynamic_collision_t->Position.x : -1000.0f;

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