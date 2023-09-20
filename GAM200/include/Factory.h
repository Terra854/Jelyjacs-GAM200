#pragma once
#include <Debug.h>

// Need to change cause Elie can tell from just a glance that it's directly plagarised from SampleEngine
// High priority to refactor this before M1 submission to avoid academic misconduct penalties
// Do not remove this until the changes are made

#include "System.h"
#include "Interface_System.h"
#include "Composition.h"
#include "Mat3.h"
#include "Vec2.h"
#include <string>
#include <map>
#include <set>
#include <string>
#include "ComponentCreator.h"

class GameObjectFactory : public ISystems
{
public:
	GameObjectFactory();
	~GameObjectFactory();

	GOC* createGameObj(const std::string& filename);

	void destroyGameObj(GOC* gameObject);

	virtual void Update(float time);

	virtual std::string SystemName() { return "Factory"; };

	//virtual void SendMessage(Message* msg);

	void destroyAllGameObjs();

	GOC* createEmptyGameObj();

	GOC* buildFromFile(const std::string& filename);

	void idGameObj(GOC* gameObj);

	GOC* getObjWithID(GOCId id);

	void AddComponentCreator(const std::string& name, ComponentCreator* creator);

	friend class Physics; // Needed to apply physics onto each object
private:
	unsigned lastGameObjID;

	//added by jia Ming
	//to map the component name in object text file to component type
	typedef std::map< std::string, ComponentCreator*> componentCreatorMap;
	componentCreatorMap componentMap;

	typedef std::map<unsigned, GOC*> gameObjIDMap;
	gameObjIDMap gameObjectMap;

	std::set<GOC*> gameObjsToBeDeleted;
};

extern GameObjectFactory* gameObjFactory;
