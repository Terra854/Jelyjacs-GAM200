#pragma once
#include <Debug.h>

#include "Interface_System.h"


class GameLogic : public ISystems
{
public:
	//Constructor to initialise pointer
	// It's broken, will be purged in the future
	//GameLogic();
	//~GameLogic();
	virtual void SendMessage(Message* msg);
	void Initialize();
	void Update(float time);
	virtual std::string SystemName() { return "Game_Logic"; }
};

extern GameLogic* Logic;



