#pragma once
#include <Object.h>
#include <GameLogic.h>

class Player : public LogicScript
{
public:
	Player(std::string);
	~Player() = default;

	void Start(Object* obj);
	void Update(Object* obj);
	void Shutdown(Object* obj);
};
