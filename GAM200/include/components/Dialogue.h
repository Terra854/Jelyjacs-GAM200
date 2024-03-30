#pragma once
#include <Debug.h>

#include "Object.h"

class Dialogue : public Component
{
public:
	Dialogue();
	void Initialize() override;
	virtual ComponentType TypeId() const override { return ComponentType::Dialogue; }

	std::vector<std::string> GetDialogueLines() const ;
	std::string GetEntireDialogue() const;
	void ChangeDialogue(std::string new_dialogue); 
	int GetSequence() const;
	void SetSequence(int);
	friend class factory;
private:
	int sequence{};
	std::vector<std::string> dialogue_lines;
	std::string entire_dialogue;
};