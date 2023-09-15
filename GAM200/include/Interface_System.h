#pragma once

//Base System class for all systems - Interface.
class ISystems {
public:
	//MessageSystem
	//virtual void SendMessage(Message* message){};
	virtual void Initialize() = 0;
	virtual void Update(float time) = 0;
	virtual void Free() = 0;

	//For Debugging
	virtual void GetSystemName() = 0;
};