#pragma once
#include <Object.h>	
#include <GameLogic.h>

class Endpoint : public LogicScript
{
public: 
	Endpoint(std::string);
	~Endpoint() = default;

	void Start(Object* obj);
	void Update(Object* obj);
	void Shutdown(Object* obj);
};