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

	// Returns name of the component
	virtual std::string SystemName() { return "DialogueSystem"; }
	bool next_dialogue{ false };
private:

};

extern DialogueSystem* dialoguesystem;