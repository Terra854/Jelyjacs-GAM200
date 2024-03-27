#pragma once

#include <Debug.h>
#include <components/Dialogue.h>
#include <Interface_System.h>


class DialogueSystem : public ISystems {
public:
	DialogueSystem();
	~DialogueSystem() {};
	virtual void Initialize();
	virtual void Update();
	//virtual void MessageRelay(Message_Handler* msg);

	// Returns name of the component
	virtual std::string SystemName() { return "DialogueSystem"; }

};

extern DialogueSystem* dialoguesystem;