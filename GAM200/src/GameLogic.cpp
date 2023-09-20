#include "GameLogic.h"
#include <iostream>
#include "Assets Manager//file_reader.h"
#include "ComponentType.h"
#include "Factory.h"
#include "ComponentCreator.h"
#include "components/Transform.h"
#include "ComponentType.h"

GameLogic* LOGIC = NULL;

void GameLogic::Initialize()
{
	GOC * testObj;
	Transform * trans;

	//In order to use the game object factory, we need to register the components we want to use first like this
	//See ComponentType.h to see what components we have for now. When we create new types of components, we need to add it in there as well
	gameObjFactory->AddComponentCreator("Transform", new ComponentCreatorType<Transform>(ComponentTypeId::CT_Transform));
	gameObjFactory->AddComponentCreator("Texture", new ComponentCreatorType<Transform>(ComponentTypeId::CT_Texture));
	gameObjFactory->AddComponentCreator("Body", new ComponentCreatorType<Transform>(ComponentTypeId::CT_Body));

	testObj = gameObjFactory->buildFromFile("test.txt");
	trans = static_cast<Transform*>( testObj->GetComponent(ComponentTypeId::CT_Transform));
	//alternate way to get component without cast
	//trans = testObj->GetComponent_NoCast<Transform>(ComponentTypeId::CT_Transform);
	trans->Mass = 0.5f;
	
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