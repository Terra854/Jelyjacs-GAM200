#pragma once
#include "interface_System.h"
#include "glslshader.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "GLWindow.h"
#include <ft2build.h>
#include FT_FREETYPE_H

enum FONT
{
	AldrichRegular,
	GeoRegular,
	total
};


//rgb values range: 0 to 1
bool DrawText(std::string const& text, float posX, float posY, float scale, float red = 1, float green = 1, float blue = 1);

int find_width(std::string const& str);

class Font : public ISystems
{
public:
	Font();
	virtual void Initialize();
	virtual void Update();
	void SetFont(FONT);
	virtual std::string SystemName() { return "Font"; }
	~Font();
};
extern Font* font;

