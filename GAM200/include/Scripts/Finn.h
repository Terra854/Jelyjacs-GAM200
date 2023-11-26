#pragma once
#include <Object.h>
#include <GameLogic.h>

class Finn : public LogicScript
{
public:
	Finn(std::string);
	~Finn() = default;

	void Start(Object* obj);
	void Update(Object* obj);
	void Shutdown(Object* obj);
};