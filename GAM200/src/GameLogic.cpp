#include "GameLogic.h"
#include <iostream>
#include "Assets Manager//file_reader.h"
#include "ComponentCreator.h"
#include "ComponentType.h"


void GameLogic::Initialize()
{
	if (!s_importMapFromFile("filename.txt"))
	{
		std::cout << "fail to import map from file" << std::endl;
	}

	//add all the game components creators into the factory map along with string key
	FACTORY->AddComponentCreator("Texture", new ComponentCreatorType<Texture>());
	FACTORY->AddComponentCreator( "Transform" , new ComponentCreatorType<Transform>());
	FACTORY->AddComponentCreator("Body", new ComponentCreatorType<Body>());
	//...etc

	//using gameobjects text file, e.g main_character.txt, extract the data of various game components, and use the previously added component creaters to create the game object.
	FACTORY->Create("game_object1_filename.txt");
	FACTORY->Create("game_object2_filename.txt");
	//...etc


}