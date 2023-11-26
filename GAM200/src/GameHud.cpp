/* !
@file GameHud.cpp
@author Yeo Jia Ming
@date	3/11/2023
//
This file contains the definition of the functions of GameHud 
*//*__________________________________________________________________________*/
#include "GameHud.h"
#include "Font.h"
#include "GLApp.h"
#include "input.h"
#include "Core_Engine.h"
#include "Assets Manager/json_serialization.h"
#include "Assets Manager/asset_manager.h"
#include "SceneManager.h"
namespace
{
	std::string button_tracker;
	bool win = false;
}

std::vector<Button> Buttons;
std::map <std::string, int> index;

int menu_index = 0;
int win_menu_index = 0;
int number_of_buttons = 0;

void draw_outline(Vec2 pos1, Vec2 pos2);

void drawline(Vec2 start, Vec2 end, glm::vec3 color);

void Buttons_change_resolution();

/******************************************************************************
	Inialise gamehud buttons and assets
*******************************************************************************/
void GameHud::Initialize()
{
	createHudFromConfig("Asset/UI/Pause.json");
	button_tracker = "nil";
}

/******************************************************************************
	checks for mouse input and update buttons
*******************************************************************************/
void GameHud::Update()
{
	
	if (input::MouseMoved())
	{
			for (int i = 0; i < Buttons.size(); ++i)
			{
				button_tracker = "nil";
				if (win && i < win_menu_index)
				{
					i = win_menu_index + 1;
				}
				else if (engine->isPaused() && i < menu_index)
				{
					i = menu_index + 1;
				}
				else if ((!engine->isPaused() && i >= menu_index && !win) || (!win && i >= win_menu_index))
				{
					break;
				}
				if (input::GetMouseX() > Buttons[i].pos1.x && input::GetMouseX() < Buttons[i].pos2.x)
				{
					if (input::GetMouseY() > Buttons[i].pos2.y && input::GetMouseY() < Buttons[i].pos1.y)
					{
						button_tracker = Buttons[i].string.text;

						break;
					}
				}
			}
	}
	
	if ((input::IsPressed(KEY::mouseL) && button_tracker != "nil"))
		{
			audio->playSfx(AudioType::Button_Click);
			if (button_tracker == "pause")
			{
				SceneManager::PauseScene();
			}
			else if (button_tracker == "resume")
			{
				SceneManager::PlayScene();
			}
			else if (button_tracker == "zoom")
			{
				if (camera2D->scale.x == 1.0f || camera2D->scale.y == 1.0f) {
					camera2D->scale = { 2.0f, 2.0f };
				}
				else camera2D->scale = { 1.0f, 1.0f };
			}
			else if (button_tracker == "resolution")
			{
				switch (window->window_size) {
					case(Window_size::fullscreen):
					{
						window->change_window_size(Window_size::high);
						window->window_size = Window_size::high;
						break;
					}
					case(Window_size::high):
					{
						window->change_window_size(Window_size::medium);
						window->window_size = Window_size::medium;
						break;
					}
					case(Window_size::medium):
					{
						window->change_window_size(Window_size::low);
						window->window_size = Window_size::low;
						break;
					}
					case(Window_size::low):
					{
						window->change_window_size(Window_size::high);
						window->window_size = Window_size::high;
						break;
					}
				}
			}
			else if (button_tracker == "fullscreen")
			{
				static bool fullscreen = false;
				if (fullscreen)
				{
					window->change_window_size(Window_size::high);
					window->window_size = Window_size::high;
				}
				else
				{
					window->change_window_size_fullscreen();
					window->window_size = Window_size::fullscreen;
				}
				fullscreen = !fullscreen;
				input::update_resolution();
				Buttons_change_resolution();
			}
			else if (button_tracker == "restart")
			{
					SceneManager::RestartScene();
					SceneManager::PlayScene();
					win = false;
			}
			else if (button_tracker == "quit")
			{
				Message_Handler msg(MessageID::Event_Type::Quit);
				engine->Broadcast(&msg);
			}
			button_tracker = "nil";
		}
}

/******************************************************************************
	draw texture and text font of the game huds
*******************************************************************************/
void GameHud::Draw()
{
	for (int i =0 ; i< Buttons.size(); ++i)
	{
		if (win && i < win_menu_index)
		{
			i = win_menu_index;
		}
		else if (engine->isPaused() && i < menu_index)
		{
			i = menu_index;
		}
		else if ((!engine->isPaused() && i >= menu_index && !win) || (!win && i >= win_menu_index))
		{
			break;
		}
		Buttons[i].draw_hud_texture();
		if (menu_index == i || win_menu_index == i)
		{
			continue;
		}
		SetFont(Buttons[i].string.font);
		DrawText(Buttons[i].string.text, Buttons[i].string.pos.x, Buttons[i].string.pos.y, Buttons[i].string.scale);
	}
	if (button_tracker != "nil" && button_tracker != "you win!")
	{
		draw_outline(Buttons[index.at(button_tracker)].pos1, Buttons[index.at(button_tracker)].pos2);
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
	if (text == "menu")
	{
		menu_index = number_of_buttons;
	}
	else if (text == "win_menu")
	{
		win_menu_index = number_of_buttons;
	}
	index[text] = number_of_buttons++;
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
		int current_width;
		int current_height;
		glfwGetFramebufferSize(GLWindow::ptr_window, &current_width, &current_height);
		posM.x = ((centre.x-17.0f) * 2.0f / current_width) ;
		posM.y = (centre.y+10.0f) * 2.0f / current_height;
		Vec2 scale;
		scale.x = (width *1.6f)/ current_width;
		scale.y = (height *2.5f)/ current_height;
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
	int current_width;
	int current_height;
	glfwGetFramebufferSize(GLWindow::ptr_window,&current_width, &current_height);
	scaling_x = abs(end.x - start.x) * 2 / current_width;
	scaling_y = abs(end.y - start.y) * 2 / current_height;
	pos_x = start.x * 2.0f / current_width;
	pos_y = start.y * 2.0f / current_height;
	Mat3 mat_test;
	mat_test = Mat3Translate(pos_x, pos_y) * Mat3Scale(scaling_x, scaling_y) * Mat3RotRad(orientation);
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


void set_win()
{
	if (!win)
	{
		win = true;
		if (!engine->isPaused())
		{
			SceneManager::PauseScene();
		}
	}
}


void Buttons_change_resolution()
{
	int current_width, current_height;
	static int old_width{ window->width_init };
	static int old_height{ window->height_init };
	glfwGetFramebufferSize(GLWindow::ptr_window, &current_width, &current_height);
	float scaleX = static_cast<float>(current_width) / old_width;
	float scaleY = static_cast<float>(current_height) / old_height;
	old_width = current_width;
	old_height = current_height;

	for (auto it = Buttons.begin(); it != Buttons.end(); ++it)
	{
		it->centre.x *= scaleX;
		it->centre.y *= scaleY;
		it->pos1.x *= scaleX;
		it->pos1.y *= scaleY;
		it-> pos2.x *= scaleX;
		it->pos2.y *= scaleY;
		it->width *= scaleX;
		it->height *= scaleY;
	}
}


