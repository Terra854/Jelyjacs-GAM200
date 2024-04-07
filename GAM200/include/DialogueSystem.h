#pragma once
/* !
@author Yeo Jia Ming
@date	23/3/2024

This file contains the dialogue system
*//*__________________________________________________________________________*/
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