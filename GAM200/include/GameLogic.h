#pragma once

#include "Interface_System.h"


class GameLogic : public ISystems
{
public:
	void Initialize();
	//void Update(float time);
	virtual std::string GetSystemName() { return "Game_Logic"; }
};



