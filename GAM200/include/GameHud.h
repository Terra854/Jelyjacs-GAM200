/* !
@file GameHud.h
@author Yeo Jia Ming
@date	3/11/2023

This file contains the declarations for the functions of gamehud class
*//*__________________________________________________________________________*/
#pragma once
/* !
@file	GameHud.h
@author	Yeo Jia Ming
@date	02/11/2023

This file contains declarations for the in-game hud
*//*__________________________________________________________________________*/
#include "Interface_System.h"
#include "Vec2.h"
#include <map>
#include "font.h"
#include "GLApp.h"

//to display the text inside the button
class Text
{
public:
	Vec2 pos{};
	float scale;
	std::string text{};
	FONT font;
};

//the dimensions of button
class Button
{
public:
	Button(Vec2 pos1, Vec2 pos2);
	Button(Vec2 centre, float width, float height);
	Vec2 pos1;
	Vec2 pos2;
	Vec2 centre;
	float width;
	float height;
	Text string{};
	//to attach an empty box texture to the button
	GLuint texture_id;
	void draw_hud_texture();
};

//map container to store all buttons created
extern std::vector<Button> Buttons;

//creates a new button with data initialised by function parameters
void create_button(std::string const& text, Button button, float scale, FONT font , GLuint id);


class GameHud
{
public:
	GameHud() = default;
	void Initialize() ;
	void Update() ;
	void Draw();
	~GameHud();
	
private:
};

void createHudFromConfig(std::string file);


