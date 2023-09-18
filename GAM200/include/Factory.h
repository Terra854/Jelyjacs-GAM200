#pragma once

#include "System.h"
#include "Interface_System.h"
#include "Composition.h"
#include "Mat3.h"
#include "Vec2.h"
#include <string>
#include <map>
#include <set>
#include <string>

class GameObjectFactory : public ISystems
{
public:
	GameObjectFactory();
	~GameObjectFactory();

	GOC* createGameObj(std::string gameObjType);

	void destroyGameObj(GOC* gameObject);

	virtual void Update(float time);

	std::string GetName() { return "Factory"; };

	//virtual void sendMessage();

	void destroyAllGameObjs();

	GOC* createEmptyGameObj();

	GOC* buildFromFile(const std::string& filename);

	void idGameObj(GOC* gameObj);

	GOC* getObjWithID(GOCId id);

private:
	unsigned lastGameObjID;

	typedef std::map<unsigned, GOC*> gameObjIDMap;
	gameObjIDMap gameObjectMap;

	std::set<GOC*> gameObjsToBeDeleted;
};

extern GameObjectFactory* gameObjFactory;
