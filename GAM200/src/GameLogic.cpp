#include <Debug.h>
#include "GameLogic.h"
#include <iostream>
#include "Assets Manager/file_reader.h"
#include "Factory.h"
#include "ComponentCreator.h"
#include "components/Transform.h"
#include "components/Texture.h"
#include "components/Body.h"

// It's broken, will be purged in the future
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
	
	
	

	//In order to use the game object factory, we need to register the components we want to use first like this
	//See ComponentType.h to see what components we have for now. 
	//When we create new types of components, we need to add it in there as well
	objectFactory->AddComponentCreator("Transform", new ComponentCreatorType<Transform>(ComponentType::Transform));
	objectFactory->AddComponentCreator("Texture", new ComponentCreatorType<Texture>(ComponentType::Texture));
	objectFactory->AddComponentCreator("Body", new ComponentCreatorType<Rectangular>(ComponentType::Body));
	
	Object* testObj;
	//Object* testObj2;
	Transform * trans;
	Texture* texture;
	
	std::cout << "test Object 1" << std::endl;
	testObj = objectFactory->buildFromFile("../Asset/Objects/TestTile.txt");
	trans = static_cast<Transform*>( testObj->GetComponent(ComponentType::Transform));
	//std::cout << "test Object 2" << std::endl;
	//testObj2 = gameObjFactory->buildFromFile("../Assest/Objects/TestTile.txt"); // testing
	
	//alternate way to get component without cast
	//trans = testObj->GetComponent_NoCast<Transform>(ComponentTypeId::CT_Transform);
	trans->Mass = 0.5f;
	//Getting a Object pointer with the game object ID
	testObj = objectFactory->getObjectWithID(0);
	texture = static_cast<Texture*>(testObj->GetComponent(ComponentType::Texture));
	
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