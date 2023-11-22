#pragma once
#include <Object.h>
#include <GameLogic.h>

class Piston : public LogicScript
{
public:
	Piston(std::string);
	~Piston() = default;

	void Start(Object* obj);
	void Update(Object* obj);
	void Shutdown(Object* obj);
};