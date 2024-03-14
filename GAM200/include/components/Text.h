#pragma once
#include <Debug.h>

#include "Object.h"

class Text : public Component
{
public:
	Text(std::string _text = "", float _fontSize = 1.f, std::string _FontType = "Aldrich-Regular") : Component(), text(_text), fontSize(_fontSize), fontType(_FontType) {}

	virtual ComponentType TypeId() const override { return ComponentType::Text; }

	std::string text;
	float fontSize = 1.f;
	std::string fontType = "Aldrich-Regular";
};