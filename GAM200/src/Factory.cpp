#include "Factory.h"
#include "Assets Manager/text_serialization.h"
#include "Vec2.h"
#include "components/Transform.h"

// Need to change cause Elie can tell from just a glance that it's directly plagarised from SampleEngine
// High priority to refactor this before M1 submission to avoid academic misconduct penalties

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

//This creates an empty game object
GOC* GameObjectFactory::createGameObj(const std::string& filename)
{
	GOC* newGameObj = buildFromFile(filename);

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
		Vec2 position;
		float size;
		float rotation;

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
				}
			}
		}
		// Id and initialize game object
		idGameObj(gameObj);

		return gameObj;
	}
	return NULL;
}

//This gives a game object an ID tag which can be used to find that same game object
void GameObjectFactory::idGameObj(GOC* gameObj)
{
	++lastGameObjID;
	gameObj->ObjectId = lastGameObjID;
	gameObjectMap[lastGameObjID] = gameObj;
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