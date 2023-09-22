#include <Debug.h>
#include "Factory.h"
#include "Assets Manager/text_serialization.h"
#include "Vec2.h"
#include "components/Transform.h"
#include <iostream>

// Need to change cause Elie can tell from just a glance that it's directly plagarised from SampleEngine
// High priority to refactor this before M1 submission to avoid academic misconduct penalties
// Do not remove this until the changes are made

/*
* GOC is what the game object is represented by. It's definition is found in Composition.h.
* As a reminder, a game object is just a group of components so the GOC will have a vector of
* components
*/

GameObjectFactory* gameObjFactory = NULL;

//Ctor
GameObjectFactory::GameObjectFactory()
{
	gameObjFactory = this;
	lastGameObjID = 0;
}

//Dtor
GameObjectFactory::~GameObjectFactory()
{
	componentCreatorMap::iterator it = componentMap.begin();
	for  (; it != componentMap.end(); ++it)
	{
		delete it->second;
	}
}

//This creates a game object with a file name. This will be what is used to create game objects in the game loop
GOC* GameObjectFactory::createGameObj(const std::string& filename)
{
	GOC* newGameObj = buildFromFile(filename);
	if (newGameObj)
	{
		newGameObj->Intialize();
	}
	return newGameObj;
}

//This doesn't destroy the game object instantly but will set it to be destroyed in the update loop
void GameObjectFactory::destroyGameObj(GOC* gameObject)
{
	gameObjsToBeDeleted.insert(gameObject);
}

//This deletes all objects to be deleted
void GameObjectFactory::Update(float dt) {
	std::set<GOC*>::iterator it = gameObjsToBeDeleted.begin();
	for (; it != gameObjsToBeDeleted.end(); ++it)
	{
		GOC* gameObject = *it;
		gameObjIDMap::iterator idItr = gameObjectMap.find(gameObject->ObjectId);
		//ErrorIf(idItr == gameObjectMap.end(), "Object %d was double deleted or is bad memory.", gameObject->ObjectId);
		if (idItr != gameObjectMap.end())
		{
			//Delete it and remove its entry in the Id map
			delete gameObject;
			gameObjectMap.erase(idItr);
		}
	}
	//All objects to be delete have been deleted
	gameObjsToBeDeleted.clear();
}
//This destroys all game objects
void GameObjectFactory::destroyAllGameObjs()
{
	gameObjIDMap::iterator it = gameObjectMap.begin();
	for (; it != gameObjectMap.end(); ++it)
	{
		delete it->second;
	}

	gameObjectMap.clear();
}

GOC* GameObjectFactory::createEmptyGameObj()
{
	GOC* gameObj = new GOC();
	idGameObj(gameObj);
	return gameObj;
}

//This creates an object from a text file. This needs to be tested a lot
GOC* GameObjectFactory::buildFromFile(const std::string& filename)
{
	//Open text file in read mode
	TextSerialization textStream;
	textStream.openFileRead(filename);

	// Make sure stream is valid
	if (textStream.isGood())
	{
		// Create new game object to hold components
		GOC* gameObj = new GOC();
		std::string componentName;

		while (textStream.isGood())
		{	
			// Read componentName
			textStream.readString(componentName);

			// Find the component Creator
			componentCreatorMap::iterator it = componentMap.find(componentName);
			if (it != componentMap.end())
			{	
				// Create Component by using the interface
				ComponentCreator* creator = it->second;
				GameComponent* component = creator->Create();

				/* commented out to test the streamGet way of serialization
				* 
				GameComponent* testComponent = component;
				// Add component to composition
				gameObj->AddComponent(creator->typeId, component);
				if (componentName == "Transform")
				{
					Transform* trans = static_cast<Transform*>(testComponent);

					Vec2 pos;
					streamGet(textStream, pos.x);
					streamGet(textStream, pos.y);
					
					trans->Position = pos;

					float scale;
					streamGet(textStream, scale);
					trans->Scale = scale;

					float rotation;
					streamGet(textStream, rotation);
					trans->Rotation = rotation;
					
				} */

				streamGet(textStream, *component);
				gameObj->AddComponent(creator->typeId, component);

			}
		}
		// Id and initialize game object
		idGameObj(gameObj);

		return gameObj;
	}
	std::cout << "textstream not good" << std::endl;
	return NULL;
}

//This gives a game object an ID tag which can be used to find that same game object
void GameObjectFactory::idGameObj(GOC* gameObj)
{
	gameObj->ObjectId = lastGameObjID;
	gameObjectMap[lastGameObjID] = gameObj;
	++lastGameObjID;
}

//This gets an object with a certain ID
GOC* GameObjectFactory::getObjWithID(GOCId id)
{
	gameObjIDMap::iterator it = gameObjectMap.find(id);
	if (it != gameObjectMap.end())
	{
		return it->second;
	}
	else
	{
		return NULL;
	}
}

//This adds a new component creator which is necessary for the creation of game objects
//Call this at the very start of the game loop in Intialize
void GameObjectFactory::AddComponentCreator(const std::string& name, ComponentCreator* creator)
{
	componentMap[name] = creator;
}