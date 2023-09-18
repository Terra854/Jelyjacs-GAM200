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