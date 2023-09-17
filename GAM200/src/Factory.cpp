#include "Factory.h"
#include "Composition.h"
#include "Assets Manager/text_serialization.h"

GameObjectFactory* gameObjFactory = NULL;

GameObjectFactory::GameObjectFactory()
{
	gameObjectFactory = this;
	lastGameObjID = 0;
}

GameObjectFactory::~GameObjectFactory()
{

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

		while (textStream.isGood())
		{

		}
	}
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