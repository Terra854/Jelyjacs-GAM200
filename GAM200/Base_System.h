#pragma once

//Base System class for all systems - Interface.
class System_Class {
public:
	//MessageSystem
	//virtual void SendMessage(Message* message){};
	virtual void Update(float time) = 0;
	virtual void GetSystemName() = 0;
	virtual void Initialize() {}
	virtual ~System_Class(){}
};