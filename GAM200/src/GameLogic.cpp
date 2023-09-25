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
#include <Movement.h>

void GameLogic::MessageRelay(Message* msg) {
	if(msg->messageId == MessageID::Movement) {
		MovementKey* temp = static_cast<MovementKey*>(msg);
		std::cout << temp->dir << std::endl;
	}
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
	Object* testObj2;
	Object* playerObj;
	Object* floor1;
	Object* floor2;
	Object* floor3;

	Object* bottom_line;
	//Transform * trans;
	//Texture* texture;
	
	std::cout << "test Object 1" << std::endl;
	testObj = objectFactory->createObject("../test.json");
	std::cout << "test Object 2" << std::endl;
	testObj2 = objectFactory->createObject("../drop-forever.json");
	std::cout << "test Player" << std::endl;
	playerObj = objectFactory->createObject("../player.json");
	floor1 = objectFactory->createObject("../mapbox.json");
	floor2 = objectFactory->createObject("../mapbox.json");
	floor3 = objectFactory->createObject("../mapbox.json");
	Transform* tran_pt = static_cast<Transform*>((objectFactory->getObjectWithID(4))->GetComponent(ComponentType::Transform));
	tran_pt->Position.x = 190; // offset objectx
	tran_pt = static_cast<Transform*>((objectFactory->getObjectWithID(5))->GetComponent(ComponentType::Transform)); // change to different obj
	tran_pt->Position.x = 380;
	tran_pt->Position.y = -300;
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
	if (input::IsPressed(KEY::w)) {
		MovementKey msg(up);
		engine->Broadcast(&msg);
	}
	if (input::IsPressed(KEY::a)) {
		MovementKey msg(left);
		engine->Broadcast(&msg);
	}
	if (input::IsPressed(KEY::s)) {
		MovementKey msg(down);
		engine->Broadcast(&msg);
	}
	if (input::IsPressed(KEY::d)) {
		MovementKey msg(right);
		engine->Broadcast(&msg);
	}
	
}