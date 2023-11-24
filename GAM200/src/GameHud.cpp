/* !
@file GameHud.cpp
@author Yeo Jia Ming
@date	3/11/2023

This file contains the definition of the functions of GameHud 
*//*__________________________________________________________________________*/
#include "GameHud.h"
#include "Font.h"
#include "GLApp.h"
#include "input.h"
#include "Core_Engine.h"
#include "Assets Manager/json_serialization.h"
#include "Assets Manager/asset_manager.h"
namespace
{
	std::vector<Button>::iterator button_tracker;

	bool keyboard_mode = false;
}

std::vector<Button> Buttons;

void draw_outline(Vec2 pos1, Vec2 pos2);

void drawline(Vec2 start, Vec2 end, glm::vec3 color);

/******************************************************************************
	Inialise gamehud buttons and assets
*******************************************************************************/
void GameHud::Initialize()
{
	//init_hud_assets();
	//create_button("play", Button(Vec2(800, 400), 180, 70), 1.0f, AldrichRegular);
	//create_button("zoom", Button(Vec2(800, 250), 180, 70), 1.2f , GeoRegular);
	createHudFromConfig("Asset/UI/Pause.json");
	button_tracker = Buttons.end();
}

/******************************************************************************
	checks for mouse input and update buttons
*******************************************************************************/
void GameHud::Update()
{
	if (input::MouseMoved())
	{
		for (auto it = Buttons.begin(); it != Buttons.end(); ++it)
		{
			if (input::GetMouseX() > it->pos1.x && input::GetMouseX() < it->pos2.x)
			{
				if (input::GetMouseY() > it->pos2.y && input::GetMouseY() < it->pos1.y)
				{
					button_tracker = it;
					keyboard_mode = false;
					break;
				}
			}
			if (!keyboard_mode)
			{
				button_tracker = Buttons.end();
			}
		}
	}
	else if (input::IsPressed(KEY::tab))
	{
		if (keyboard_mode && button_tracker < Buttons.end() - 1)
		{
			++button_tracker;
		}
		else
		{
			button_tracker = Buttons.begin();
		}
		keyboard_mode = true;
	}

		if ((input::IsPressed(KEY::mouseL) && button_tracker != Buttons.end() && !keyboard_mode) || (input::IsPressed(KEY::enter) && keyboard_mode))
		{
			if (button_tracker->string.text == "play" || button_tracker->string.text == "pause")
			{
				if (!engine->isPaused())
				{
					button_tracker->string.text = "play";
					button_tracker->string.pos.x = button_tracker->centre.x - (static_cast<float>(find_width("play", button_tracker->string.font)) / 2.0f * button_tracker->string.scale);
				}
				else
				{
					button_tracker->string.text = "pause";
					button_tracker->string.pos.x = button_tracker->centre.x - (static_cast<float>(find_width("pause", button_tracker->string.font)) / 2.0f * button_tracker->string.scale);
				}
				engine->setPause();
			}

			else if (button_tracker->string.text == "zoom")
			{
				if (camera2D->scale.x == 1.0f || camera2D->scale.y == 1.0f) {
					camera2D->scale = { 2.0f, 2.0f };
				}
				else camera2D->scale = { 1.0f, 1.0f };
			}
		}
}

/******************************************************************************
	draw texture and text font of the game huds
*******************************************************************************/
void GameHud::Draw()
{
	for (auto it = Buttons.begin(); it != Buttons.end(); ++it)
	{
		std::cout << "width" << it->width << std::endl;
		std::cout <<"height" << it->height << std::endl;
	}
	for (auto it = Buttons.begin() ; it!=Buttons.end(); ++it)
	{
		it->draw_hud_texture();
		SetFont(it->string.font);
		DrawText(it->string.text, it->string.pos.x, it->string.pos.y, it->string.scale);
	}
	if (button_tracker != Buttons.end())
	{
		draw_outline(button_tracker->pos1, button_tracker->pos2);
	}
}

void create_button(std::string const& text, Button button, float scale , FONT f, GLuint id)
{
	button.string.pos.x = button.centre.x - static_cast<float>(find_width(text,f))/2 * scale ;
	button.string.pos.y -= 14 * scale;
	button.string.text = text;
	button.string.scale = scale;
	button.string.font = f;
	button.texture_id = id;
	Buttons.push_back(button);
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

/******************************************************************************
	dtor
*******************************************************************************/
GameHud::~GameHud()
{
	Buttons.clear();
}

void Button::draw_hud_texture()
{
		glBindTextureUnit(6, texture_id);
		glBindTexture(GL_TEXTURE_2D, texture_id);
		// load shader program in use by this object
		GLApp::shdrpgms["image"].Use();
		glBindVertexArray(GLApp::models["square"].vaoid);
		// copy object's model-to-NDC matrix to vertex shader's
		// uniform variable uModelToNDC
		Vec2 posM;
		posM.x = ((centre.x-17.0f) * 2.0f / window->width_init  ) ;
		posM.y = (centre.y+10.0f) * 2.0f / window->height_init;
		Vec2 scale;
		scale.x = (width *1.6f)/ window->width_init;
		scale.y = (height *2.5f)/ window->height_init;
		Mat3 mat = Mat3Translate(posM) * Mat3Scale(scale) * Mat3RotRad(0.f);
		Vec2 window_sacling = { (float)window->width / window->width_init, (float)window->height / window->height_init };
		mat = Mat3Scale(window_sacling.x, window_sacling.y) * mat;
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

void draw_outline(Vec2 pos1 , Vec2 pos2)
{
	drawline(pos1, Vec2(pos2.x, pos1.y), glm::vec3(1, 1, 1));
	drawline(pos2, Vec2(pos1.x, pos2.y), glm::vec3(1, 1, 1));
	drawline(pos1, Vec2(pos1.x, pos2.y), glm::vec3(1, 1, 1));
	drawline(pos2, Vec2(pos2.x, pos1.y), glm::vec3(1, 1, 1));
}



void drawline(Vec2 start, Vec2 end, glm::vec3 color) {
	float pos_x;
	float pos_y;
	float orientation;
	float scaling_x;
	float scaling_y;
	orientation = atan2(end.y - start.y, end.x - start.x);

	scaling_x = abs(end.x - start.x) * 2 / window->width_init;
	scaling_y = abs(end.y - start.y) * 2 / window->height_init;
	pos_x = start.x * 2.0f / window->width_init;
	pos_y = start.y * 2.0f / window->height_init;

	Mat3 mat_test;
	mat_test = Mat3Translate(pos_x, pos_y) * Mat3Scale(scaling_x, scaling_y) * Mat3RotRad(orientation);
	Vec2 window_sacling = { (float)window->width / window->width_init, (float)window->height / window->height_init };
	mat_test = Mat3Scale(window_sacling.x, window_sacling.y) * mat_test;
	//draw line
	GLApp::shdrpgms["shape"].Use();
	// bind VAO of this object's model
	glBindVertexArray(GLApp::models["line"].vaoid);
	// copy object's model-to-NDC matrix to vertex shader's
	// uniform variable uModelToNDC
	GLApp::shdrpgms["shape"].SetUniform("uModel_to_NDC", mat_test.ToGlmMat3());
	GLApp::shdrpgms["shape"].SetUniform("uColor", color);
	// call glDrawElements with appropriate arguments
	glDrawElements(GLApp::models["line"].primitive_type, GLApp::models["line"].draw_cnt, GL_UNSIGNED_SHORT, 0);

	// unbind VAO and unload shader program
	glBindVertexArray(0);
	GLApp::shdrpgms["shape"].UnUse();
}

void createHudFromConfig(std::string file)
{
	JsonSerialization jsonobj;
	jsonobj.openFileRead(file);

	for (auto& button : jsonobj.read("Button"))
	{
		JsonSerialization jsonloop;
		jsonloop.jsonObject = &button;

		std::string text, font, texture;
		jsonloop.readString(text, "text");
		float x, y, w, h, scale;
		jsonloop.readFloat(x, "button_x");
		jsonloop.readFloat(y, "button_y");
		jsonloop.readFloat(w, "button_w");
		jsonloop.readFloat(h, "button_h");
		jsonloop.readFloat(scale, "scale");

		jsonloop.readString(font, "font");
		FONT f = total;
		if (font == "AldrichRegular")
		{
			f = FONT::AldrichRegular;
		}
		else if (font == "GeoRegular")
		{
			f = FONT::GeoRegular;
		}

		GLuint val;
		jsonloop.readString(texture, "texture");
		val = AssetManager::textureval(texture);

		if (!AssetManager::texturecheckexist(texture))
			std::cout << "TEXTURE MISSING FOR BUTTON!!!\n";

		create_button(text, Button(Vec2(x, y), w, h), scale, f, val);
	}


	jsonobj.closeFile();
}








