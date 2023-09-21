#include <Debug.h>
#include "GameLogic.h"
#include <iostream>
#include "Assets Manager//file_reader.h"
#include "ComponentType.h"
#include "Factory.h"
#include "ComponentCreator.h"
#include "components/Transform.h"
#include "components/Texture.h"
#include "ComponentType.h"

//GameLogic* Logic = NULL;
/*
GameLogic::GameLogic() {
	Logic = this;
}
GameLogic::~GameLogic() {

}
*/

void GameLogic::SendMessage(Message* msg) {
	// Keys to Game Logic
}
void GameLogic::Initialize()
{
	//Logic = this;
	GOC * testObj;
	GOC* testObj2;
	Transform * trans;
	Texture* texture;

	//In order to use the game object factory, we need to register the components we want to use first like this
	//See ComponentType.h to see what components we have for now. 
	//When we create new types of components, we need to add it in there as well
	gameObjFactory->AddComponentCreator("Transform", new ComponentCreatorType<Transform>(ComponentTypeId::CT_Transform));
	gameObjFactory->AddComponentCreator("Texture", new ComponentCreatorType<Transform>(ComponentTypeId::CT_Texture));
	gameObjFactory->AddComponentCreator("Body", new ComponentCreatorType<Transform>(ComponentTypeId::CT_Body));

	testObj = gameObjFactory->buildFromFile("test.txt");
	trans = static_cast<Transform*>( testObj->GetComponent(ComponentTypeId::CT_Transform));

	GOC* testobj2 = gameObjFactory->buildFromFile("../../Asset/Objects/TestTile.txt"); // testing

	//alternate way to get component without cast
	//trans = testObj->GetComponent_NoCast<Transform>(ComponentTypeId::CT_Transform);
	trans->Mass = 0.5f;
	//Getting a GOC pointer with the game object ID
	testObj2 = gameObjFactory->getObjWithID(0);
	texture = static_cast<Texture*>(testObj->GetComponent(ComponentTypeId::CT_Texture));
	
	//Broken code. DO NOT REMOVE YET - Jonathan
	/*
	gameObjFactory->AddComponentCreator("Transform", new ComponentCreatorType<Transform>(ComponentTypeId::CT_Transform));

	ComponentCreatorType<Transform> transformComponetCreator(ComponentTypeId::CT_Transform);
	ComponentCreator* transformPtr = transformComponetCreator;

	GameObjectFactory gameObjFactory;
	std::string transform = "Transform";

	gameObjFactory.AddComponentCreator(transform, transformPtr);
	*/
}

void GameLogic::Update(float time) {

}