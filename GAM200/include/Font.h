/* !
@file Font.h
@author Yeo Jia Ming
@date	3/11/2023

This file contains the declarations of the functions for font system
*//*__________________________________________________________________________*/
#pragma once
#include "interface_System.h"
#include "glslshader.h"
#define GLM_FORCE_SILENT_WARNINGS
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "GLWindow.h"
#include <ft2build.h>
#include FT_FREETYPE_H


//rgb values range: 0 to 1
bool DrawText(std::string const& text, float posX, float posY, float scale, float red = 1, float green = 1, float blue = 1);

//calculate the width of text string in pixels, default scale, 1
int find_width(std::string const& str ,std::string font);

//fonts:
//AldrichRegular
void SetFont(std::string font = "Aldrich-Regular");




void setup_font_vao(GLuint vaoid,GLuint vboid);


struct Character {
    unsigned int TextureID;  // ID handle of the glyph texture
    glm::ivec2   Size;       // Size of glyph
    glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
    unsigned int Advance;    // Offset to advance to next glyph
};

//the entire set of data needed to draw a string of text in a particular font
struct outline
{
    //container of ascii chars map to character alignments
    std::map<char, Character> Characters;

    //metrics for the font
    FT_Face face{};

    //member function to set a mormalised size in pixels
    void set_pixel_size(int size);

    //to initialise the map of chars 
    void load_ascii_chars();
};


//font system
class Font : public ISystems
{
public:
	Font();
	virtual void Initialize();
	virtual void Update();
	virtual std::string SystemName() { return "Font"; }
	~Font();
};
extern Font* FontSystem;

