#pragma once
#include <Debug.h>

#include "Interface_System.h"


class GameLogic : public ISystems
{
public:
	void Initialize();
	//void Update(float time);
	virtual std::string SystemName() { return "Game_Logic"; }
};



