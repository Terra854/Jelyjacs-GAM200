/* !
@file	GameHud.cpp
@author	Yeo Jia Ming
@date	02/11/2023

This file contains the functions needed to render the in-game hud
*//*__________________________________________________________________________*/
#include "GameHud.h"
#include "Font.h"
#include "GLApp.h"
#include "input.h"
#include "Core_Engine.h"
namespace
{
	class Text
	{
	public:
		Vec2 pos{};
		float scale;
		std::string text{};
		FONT font;
	};
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
	};
	std::map<std::string, Button*> Buttons;



GLuint texture_id;
}
void create_button(std::string const& text, Button button, float scale, FONT font);

void init_hud_assets();

void draw_hud_texture(Vec2 pos, float scaleX, float scaleY);

void GameHud::Initialize()
{
	init_hud_assets();
	create_button("play", Button(Vec2(800, 400), 180, 70), 1.0f, AldrichRegular);
	create_button("zoom", Button(Vec2(800, 250), 180, 70), 1.2f , GeoRegular);

}

void GameHud::Update()
{
	if (input::IsPressed(KEY::mouseL))
	{
		for (auto it = Buttons.begin(); it != Buttons.end(); ++it)
		{
			if (it->first == "play" || it->first == "pause")
			{
				if (!engine->isPaused())
				{
					it->second->string.text = "play";
					it->second->string.pos.x = it->second->centre.x - (static_cast<float>(find_width("play", it->second->string.font)) / 2.0f * it->second->string.scale);
				}
				else
				{
					it->second->string.text = "pause";
					it->second->string.pos.x = it->second->centre.x - (static_cast<float>(find_width("pause", it->second->string.font)) / 2.0f * it->second->string.scale);
				}
			}

			Button* ptr = it->second;
			if (input::GetMouseX() < ptr->pos1.x || input::GetMouseX() > ptr->pos2.x || input::GetMouseY() < ptr->pos2.y || input::GetMouseY() > ptr->pos1.y)
			{
				continue;
			}
			std::cout << it->first << " is pressed" << std::endl;
			if (it->first == "play" || it->first == "pause")
			{
				engine->setPause();
			}
			else if (it->first == "zoom")
			{
				if (camera2D->scale.x == 1.0f || camera2D->scale.y == 1.0f) {
					camera2D->scale = { 2.0f, 2.0f };
				}
				else camera2D->scale = { 1.0f, 1.0f };
			}
		}
	}
}

void create_button(std::string const& text, Button button, float scale , FONT f)
{
	button.string.pos.x = button.centre.x - static_cast<float>(find_width(text,f))/2 * scale ;
	button.string.pos.y -= 14 * scale;
	button.string.text = text;
	button.string.scale = scale;
	button.string.font = f;
	Buttons[text] = new Button{ button };
}

Button::Button(Vec2 Pos1,Vec2 Pos2)
	:pos1{Pos1}, pos2{Pos2}
{
	centre = (pos1 + pos2) / 2;
	width = pos1.x - pos2.x;
	height = pos1.y - pos2.y;
	string.pos.x = pos1.x + ((pos1.x + pos2.x) / 2);
	string.pos.y = pos1.y + ((pos1.y + pos2.y) / 2);
}

Button::Button(Vec2 pos, float width, float height)
{
	centre = pos;
	Button::width = width;
	Button::height = height;
	string.pos = pos;
	pos1.x = pos.x - width / 2;
	pos2.x = pos.x + width / 2;
	pos1.y = pos.y + height / 2;
	pos2.y = pos.y - height / 2;
}

void GameHud::Draw()
{
	for (auto it = Buttons.begin() ; it!=Buttons.end(); ++it)
	{
		Button* ptr = it->second;
		draw_hud_texture(ptr->centre ,ptr->width , ptr->height);
		SetFont(ptr->string.font);
		DrawText(ptr->string.text, ptr->string.pos.x, ptr->string.pos.y, ptr->string.scale);
	}
}

GameHud::~GameHud()
{
	for (auto it = Buttons.begin(); it != Buttons.end(); ++it)
	{
		delete it->second;
	}
	Buttons.clear();
}

void draw_hud_texture( Vec2 pos , float scaleX , float scaleY)
{
		glBindTextureUnit(6, texture_id);
		glBindTexture(GL_TEXTURE_2D, texture_id);
		// load shader program in use by this object
		GLApp::shdrpgms["image"].Use();
		glBindVertexArray(GLApp::models["square"].vaoid);
		// copy object's model-to-NDC matrix to vertex shader's
		// uniform variable uModelToNDC
		Vec2 posM;
		posM.x = ((pos.x-17.0f) * 2.0f / window->width  ) ;
		posM.y = (pos.y+10.0f) * 2.0f / window->height;
		Vec2 scale;
		scale.x = (scaleX*1.6f)/ window->width ;
		scale.y = (scaleY *2.5f)/ window->height;
		Mat3 mat = Mat3Translate(posM) * Mat3Scale(scale) * Mat3RotRad(0.f);
		GLApp::shdrpgms["image"].SetUniform("uModel_to_NDC", mat.ToGlmMat3());
		// tell fragment shader sampler uTex2d will use texture image unit 6
		GLuint tex_loc = glGetUniformLocation(GLApp::shdrpgms["image"].GetHandle(), "uTex2d");
		glUniform1i(tex_loc, 6);
		// call glDrawElements with appropriate arguments
		glDrawElements(GLApp::models["square"].primitive_type, GLApp::models["square"].draw_cnt, GL_UNSIGNED_SHORT, 0);
		// unbind VAO and unload shader program
		glBindVertexArray(0);
		GLApp::shdrpgms["image"].UnUse();
}

void init_hud_assets()
{
	texture_id = GLApp::setup_texobj("Asset/Picture/Empty_Box.png");
}


