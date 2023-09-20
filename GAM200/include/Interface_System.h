#pragma once
#include <Debug.h>
#include <string>
#include <message.h>

//Base System class for all systems - Interface.
class ISystems {
public:
	//MessageSystem
	virtual void SendMessage(Message* message) {};
	virtual void Initialize(){};
	virtual void Update(float time)=0;
	virtual void Free(){};

	//For Debugging
	virtual std::string GetSystemName() { return "ISystems (If you are seeing this, you need to implement GetSystemName() inside yor component)"; };
};