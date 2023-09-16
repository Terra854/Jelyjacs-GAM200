#pragma once

//Base System class for all systems - Interface.
class ISystems {
public:
	//MessageSystem
	//virtual void SendMessage(Message* message){};
	virtual void Initialize(){};
	virtual void Update(float time){};
	virtual void Free(){};

	//For Debugging
	virtual void GetSystemName(){};
};