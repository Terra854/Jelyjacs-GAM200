#pragma once
/* !
@file	GameHud.h
@author	Yeo Jia Ming , Tay Sen Chuan , Guo chen
@date	27/11/2023

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
	std::string text{};
	Vec2 scale;
	FONT font = FONT::AldrichRegular;
};

//the dimensions of button
class Button
{
public:
	Button(Vec2 centre, float width, float height);
	Vec2 centre;
	float width;
	float height;
	Text string{};
	bool selected{};
	GLuint texture_id;
	Vec2 get_pos1() const;
	Vec2 get_pos2() const;
};

struct Menu
{
	Menu();
	Menu(float , float ,float , float, GLuint);
	Vec2 pos;
	Vec2 scale;
	GLuint texture_id;
	void init();
};

//change to winning screen
//void set_win();

class GameHud
{
public:
	GameHud() = default;
	void Initialize() ;
	void Update() ;
	void Draw();
	void Menu();
	~GameHud();
	
private:
};




