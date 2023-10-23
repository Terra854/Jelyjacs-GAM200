#pragma once
#include "interface_System.h"
#include "glslshader.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "GLWindow.h"
#include <ft2build.h>
#include FT_FREETYPE_H

bool DrawText(std::string const& text, float posX, float posY, float scale);

class Font : public ISystems
{
public:
	Font();
	virtual void Initialize();
	virtual void Update();
	virtual std::string SystemName() { return "Font"; }
};
extern Font* font;

