#pragma once
#include <Object.h>
#include <GameLogic.h>

class Player : public LogicScript
{
public:
	Player(std::string);
	~Player() = default;

	void Start(Object* obj) override;
	void Update(Object* obj) override;
	void Shutdown(Object* obj) override;
};
