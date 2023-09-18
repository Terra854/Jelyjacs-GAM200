#include "Factory.h"
#include "Assets Manager/text_serialization.h"

GameObjectFactory* gameObjFactory = NULL;

GameObjectFactory::GameObjectFactory()
{
	gameObjFactory = this;
	lastGameObjID = 0;
}

GameObjectFactory::~GameObjectFactory()
{
	componentCreatorMap::iterator it = componentMap.begin();
	for  (; it != componentMap.end(); ++it)
	{
		delete it->second;
	}
}

GOC* GameObjectFactory::createGameObj(std::string gameObjType)
{
	GOC* newGameObj = NULL;

	return newGameObj;
}

void GameObjectFactory::destroyGameObj(GOC* gameObject)
{
	gameObjsToBeDeleted.insert(gameObject);
}
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

GOC* GameObjectFactory::buildFromFile(const std::string& filename)
{
	TextSerialization textStream;
	textStream.openFileRead(filename);

	if (textStream.isGood())
	{
		GOC* gameObj = new GOC();
		std::string componentName;

		while (textStream.isGood())
		{
			textStream.readString(componentName);

			componentCreatorMap::iterator it = componentMap.find(componentName);
			if (it != componentMap.end())
			{
				ComponentCreator* creator = it->second;
				GameComponent* component = creator->Create();

				gameObj->AddComponent(creator->typeId, component);
			}
		}

		idGameObj(gameObj);

		return gameObj;
	}
	return NULL;
}

void GameObjectFactory::idGameObj(GOC* gameObj)
{
	++lastGameObjID;
	gameObj->ObjectId = lastGameObjID;
	gameObjectMap[lastGameObjID] = gameObj;
}

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

void GameObjectFactory::AddComponentCreator(const std::string& name, ComponentCreator* creator)
{
	componentMap[name] = creator;
}