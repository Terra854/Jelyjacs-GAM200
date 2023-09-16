#pragma once
#include <string>

//I think we might need this as all systems will need to inherit some common stuff like passing on messages and stuff like that
class ISystem
{
public:
	virtual void SendMessage();

	virtual void Update(float time) = 0;

	virtual std::string GetName();
};