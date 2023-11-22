#pragma once
#include <Object.h>
#include <GameLogic.h>

class Vert_Elevator : public LogicScript
{
public:
	Vert_Elevator(std::string);
	~Vert_Elevator() = default;

	void Start(Object* obj) override;
	void Update(Object* obj) override;
	void Shutdown(Object* obj) override;

private:
	bool moving_platform_direction;
};