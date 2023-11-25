#pragma once
#include <Object.h>
#include <GameLogic.h>

class CatPortal : public LogicScript
{
public:
	CatPortal(std::string);
	~CatPortal() = default;

	void Start(Object* obj);
	void Update(Object* obj);
	void Shutdown(Object* obj);

	static bool justTeleported;
};