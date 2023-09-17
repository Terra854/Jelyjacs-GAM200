#include "System.h"
#include "Composition.h"
#include "Mat3.h"
#include "Vec2.h"
#include <string>
#include <map>
#include <set>
#include <string>

class GameObjectFactory : public ISystem
{
public:
	GameObjectFactory();
	~GameObjectFactory();

	GOC* createGameObj(std::string gameObjType);

	void destroyGameObj(GOC* gameObject);

	virtual void Update(float dt);

	std::string GetName() { return "Factory"; };

	void sendMessage();

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

extern GameObjectFactory* gameObjectFactory;
