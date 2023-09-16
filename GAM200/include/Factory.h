#include "System.h"
#include "Composition.h"
#include "Mat3.h"
#include "Vec2.h"
#include <string>
#include <map>
#include <set>

class GameObjectFactory : public ISystem
{
public:
	GameObjectFactory();
	~GameObjectFactory();

	GOC* CreateGameObj();

	void DestroyGameObj(GOC* gameObject);

	std::string GetName() { return "Factory"; };

	void SendMessage();

	void DestroyAllGameObjs();

private:
	unsigned lastGameObjID;

	typedef std::map<unsigned, GOC*> gameObjMap;
	gameObjMap gameObjectMap;

	std::set<GOC*> gameObjsToBeDeleted;
};
