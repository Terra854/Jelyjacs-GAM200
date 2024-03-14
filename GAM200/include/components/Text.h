#pragma once
#include <Debug.h>

#include "Object.h"

class Text : public Component
{
public:
	Text(std::string _text = "", float _fontSize = 1.f, int _FontType = 0) : Component(), text(_text), fontSize(_fontSize), fontType(_FontType) {}

	virtual ComponentType TypeId() const override { return ComponentType::Text; }

	std::string text;
	float fontSize = 1.f;
	int fontType = 0;
};