#pragma once
#include <Debug.h>
#include <string>
#include <message.h>

//Base System class for all systems - Interface.
class ISystems {
public:
	//Messaging System for inter System communication
	virtual void SendMessage(Message* message) {};
	// System Initialization
	virtual void Initialize(){};
	// System Update
	virtual void Update(float time)=0;
	// System freeing resources
	virtual void Free(){};

	//For Debugging
	virtual std::string SystemName() { return "ISystems (If you are seeing this, you need to implement SystemName() inside yor component)"; };
};