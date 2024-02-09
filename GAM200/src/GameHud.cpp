/* !
@file GameHud.cpp
@author Yeo Jia Ming, Tay Sen Chuan , Guo chen
@date	27/11/2023
//
This file contains the definition of the functions of GameHud 
*//*__________________________________________________________________________*/
#include <Debug.h>
#include "GameHud.h"
#include "Font.h"
#include "GLApp.h"
#include "input.h"
#include "Core_Engine.h"
#include "Assets Manager/json_serialization.h"
#include "Assets Manager/asset_manager.h"
#include "SceneManager.h"
#include "../../src/Assets Manager/asset_manager.h"
enum PAGE
{
	play,
	pause_menu,
	settings_menu,
	main_menu,
	menu_level_select,
	total_page
} current_page;

std::vector <std::vector<Button>> all_buttons(total_page);

std::map<std::string,std::vector<Menu>> menu_background;

Button* button_tracker;
std::string menu_tracker;

bool win = false;
float counter = 0.f;

void create_buttons_from_config(std::string file);
void create_menus_from_config(std::string file);
Button setup_button(Vec2 centre, float width, float height, Vec2 scale, std::string string, GLuint textreid, FONT f);
void drawline(Vec2 start, Vec2 end, glm::vec3 color);
void draw_outline(Vec2 pos1, Vec2 pos2);
void draw_Button_texture(Button b);
void draw_menu(Menu m);
void set_button_logic();
void set_background();
void Buttons_update_resolution();

/******************************************************************************
	Inialise gamehud buttons and assets
*******************************************************************************/
void GameHud::Initialize()
{
	current_page = PAGE::main_menu;
	create_buttons_from_config("Asset/UI/Pause.json");
	create_menus_from_config("Asset/UI/MainMenu.json");
}

/******************************************************************************
	checks for mouse input and update buttons
*******************************************************************************/
void GameHud::Update()
{
	button_tracker = nullptr;

	for (size_t i = 0; i < all_buttons.at(current_page).size(); ++i)
	{
		if (input::GetMouseX() < all_buttons.at(current_page).at(i).get_pos1().x || input::GetMouseX() > all_buttons.at(current_page).at(i).get_pos2().x || input::GetMouseY() < all_buttons.at(current_page).at(i).get_pos2().y || input::GetMouseY() > all_buttons.at(current_page).at(i).get_pos1().y)
		{
			continue;
		}
		button_tracker = &all_buttons.at(current_page).at(i);
	}

	if (button_tracker != nullptr)
	{
		if (input::IsPressed(KEY::mouseL))
		{
			set_button_logic();
		}
	}
	set_background();

	Buttons_update_resolution();

}

/******************************************************************************
	draw texture and text font of the game huds
*******************************************************************************/
void GameHud::Draw()
{
	if (menu_tracker != "")
	{
		for (size_t i = 0; i < menu_background[menu_tracker].size(); ++i)
		{
			draw_menu(menu_background[menu_tracker].at(i));
		}
	}

	for (size_t j = 0; j < all_buttons.at(current_page).size(); ++j)
	{
		draw_Button_texture(all_buttons.at(current_page).at(j));
		if (all_buttons.at(current_page).at(j).string.font != FONT::total)
		{
			SetFont(all_buttons.at(current_page).at(j).string.font);
			DrawText(all_buttons.at(current_page).at(j).string.text, all_buttons.at(current_page).at(j).string.pos.x, all_buttons.at(current_page).at(j).string.pos.y, 1.0f);
		}
	}

		
	if (button_tracker != nullptr)
	{
		draw_outline(button_tracker->get_pos1(), button_tracker->get_pos2());
	}
	if (counter > 3.f) {
		win = false;
		counter = 0.f;
		sceneManager->LoadScene("Asset/Levels/level_1.json");
		current_page = PAGE::main_menu;
	}

	if (win) {
		SetFont(FONT::AldrichRegular);
		DrawText("You Win", 0, 0, 1.0f);
		counter += engine->GetDt();
		std::cout << counter << std::endl;
	}

	
	
	
}

Button::Button(Vec2 pos, float width_, float height_)
	:centre{pos}, width{ width_}, height{height_},
	string{}, selected{ false }, texture_id{ 0 }
{
}
Vec2 Button::get_pos1() const
{
	return Vec2(centre.x - width / 2, centre.y + height/2);
}
Vec2 Button::get_pos2() const
{
	return Vec2(centre.x + width / 2, centre.y - height / 2);
}


/******************************************************************************
	dtor
*******************************************************************************/
GameHud::~GameHud()
{
}

void set_win()
{
	win = true;
}

void create_buttons_from_config(std::string file)
{
	
	JsonSerialization jsonobj;
	jsonobj.openFileRead(file);

	std::array<std::string, 5> category
	{ "play" , "pause" , "settings" , "main_menu" , "menu_level_select"};

	for (size_t i=0 ; i<category.size() ; ++i)
	{
		//text, texture, pos, width, height 
		for (auto& button : jsonobj.read(category.at(i)))
		{
			JsonSerialization jsonloop;
			jsonloop.jsonObject = &button;

			std::string text, font, texture;
			jsonloop.readString(text, "text");
			float x, y, w, h, scale_x , scale_y;
			jsonloop.readFloat(x, "button_x");
			jsonloop.readFloat(y, "button_y");
			jsonloop.readFloat(w, "button_w");
			jsonloop.readFloat(h, "button_h");
			jsonloop.readFloat(scale_x, "scale_x");
			jsonloop.readFloat(scale_y, "scale_y");
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
			else
			{
			}

			GLuint val;
			jsonloop.readString(texture, "texture");
			val = AssetManager::textureval(texture);

			if (!AssetManager::texturecheckexist(texture))
				std::cout << "TEXTURE MISSING FOR BUTTON!!!\n";

			all_buttons.at(i).push_back(setup_button(Vec2(x, y), w, h, Vec2(scale_x , scale_y), text, val, f));
		}
	}
	jsonobj.closeFile();
	
}



void create_menus_from_config(std::string file)
{
	JsonSerialization jsonobj;
	if (!jsonobj.openFileRead(file))
	{
		std::cout << "can't open file" << std::endl;
	}

	std::array< std::string, 2> categories
	{"Menu" , "Pause_menu"};
	for (size_t i = 0; i < categories.size(); ++i)
	{
		std::vector<Menu> tmp;
		for (auto& menu : jsonobj.read(categories.at(i)))
		{
			JsonSerialization jsonloop;
				jsonloop.jsonObject = &menu;

				std::string texture, text;
				float x, y, w, h;
				jsonloop.readString(text, "text");
				jsonloop.readFloat(x, "position_x");
				jsonloop.readFloat(y, "position_y");
				jsonloop.readFloat(w, "scale_x");
				jsonloop.readFloat(h, "scale_y");

			GLuint val;
			jsonloop.readString(texture, "texture");
			val = AssetManager::textureval(texture);

			if (!AssetManager::texturecheckexist(texture))
			{
				std::cout << "TEXTURE MISSING FOR MENU!!!\n";
			}
			
			tmp.push_back(Menu(x, y, w, h, val));
		}
		menu_background[categories.at(i)] = tmp;
	}
	jsonobj.closeFile();
}


Button setup_button(Vec2 centre, float width, float height, Vec2 scale, std::string string_, GLuint textreid, FONT f)
{
	Button b(centre, width, height);
	if (f != FONT::total)
	{
		b.string.pos.x = centre.x - static_cast<float>(find_width(string_, f)) / 2;
		b.string.pos.y = centre.y - 14;
	}
	b.string.scale = scale;
	b.string.font = f;
	b.string.text = string_;
	b.texture_id = textreid;
	return b;
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
	glfwGetFramebufferSize(GLWindow::ptr_window, &current_width, &current_height);
	scaling_x = abs(end.x - start.x) * 2 / current_width;
	scaling_y = abs(end.y - start.y) * 2 / current_height;
	pos_x = start.x * 2.0f / current_width;
	pos_y = start.y * 2.0f / current_height;
	Mat3 mat_test;
	mat_test = Mat3Translate(pos_x, pos_y) * Mat3Scale(scaling_x, scaling_y) * Mat3RotRad(orientation);
	//draw line
	AssetManager::shaderval("shape").Use();
	// bind VAO of this object's model
	glBindVertexArray(AssetManager::modelval("line").vaoid);
	// copy object's model-to-NDC matrix to vertex shader's
	// uniform variable uModelToNDC
	AssetManager::shaderval("shape").SetUniform("uModel_to_NDC", mat_test.ToGlmMat3());
	AssetManager::shaderval("shape").SetUniform("uColor", color);
	// call glDrawElements with appropriate arguments
	glDrawElements(AssetManager::modelval("line").primitive_type, AssetManager::modelval("line").draw_cnt, GL_UNSIGNED_SHORT, 0);

	// unbind VAO and unload shader program
	glBindVertexArray(0);
	AssetManager::shaderval("shape").UnUse();
}

void draw_outline(Vec2 pos1, Vec2 pos2)
{
	drawline(pos1, Vec2(pos2.x, pos1.y), glm::vec3(1, 1, 1));
	drawline(pos2, Vec2(pos1.x, pos2.y), glm::vec3(1, 1, 1));
	drawline(pos1, Vec2(pos1.x, pos2.y), glm::vec3(1, 1, 1));
	drawline(pos2, Vec2(pos2.x, pos1.y), glm::vec3(1, 1, 1));
}



void draw_Button_texture( Button b)
{
	glBindTextureUnit(6, b.texture_id);
	glBindTexture(GL_TEXTURE_2D, b.texture_id);
	// load shader program in use by this object

	AssetManager::shaderval("image").Use();

	glBindVertexArray(AssetManager::modelval("square").vaoid);
	// copy object's model-to-NDC matrix to vertex shader's
	// uniform variable uModelToNDC
	Vec2 posM;
	int current_width;
	int current_height;
	glfwGetFramebufferSize(GLWindow::ptr_window, &current_width, &current_height);
	posM.x = b.centre.x * 2 / current_width;
	posM.y = b.centre.y * 2 / current_height;
	Vec2 scale;
	scale.x = b.string.scale.x / current_width;
	scale.y = b.string.scale.y / current_height;
	Mat3 mat = Mat3Translate(posM) * Mat3Scale(scale) * Mat3RotRad(0.f);
	AssetManager::shaderval("image").SetUniform("uModel_to_NDC", mat.ToGlmMat3());
	// tell fragment shader sampler uTex2d will use texture image unit 6
	GLuint tex_loc = glGetUniformLocation(AssetManager::shaderval("image").GetHandle(), "uTex2d");
	glUniform1i(tex_loc, 6);
	// call glDrawElements with appropriate arguments
	glDrawElements(AssetManager::modelval("square").primitive_type, AssetManager::modelval("square").draw_cnt, GL_UNSIGNED_SHORT, 0);
	// unbind VAO and unload shader program
	glBindVertexArray(0);
	AssetManager::shaderval("image").UnUse();
}

void set_button_logic()
{
	if ("pause" == button_tracker->string.text)
	{
		current_page = PAGE::pause_menu;
		SceneManager::PauseScene();
	}
	else if ("zoom" == button_tracker->string.text)
	{
		if (camera2D->scale.x == 1.0f || camera2D->scale.y == 1.0f) {
			camera2D->scale = { 2.0f, 2.0f };
		}
		else camera2D->scale = { 1.0f, 1.0f };
	}
	else if ("resume" == button_tracker->string.text)
	{
		current_page = PAGE::play;
		SceneManager::PlayScene();
	}
	else if ("restart" == button_tracker->string.text)
	{
		SceneManager::RestartScene();
		SceneManager::PlayScene();
		current_page = PAGE::play;
	}
	else if ("quit game" == button_tracker->string.text)
	{
		current_page = PAGE::main_menu;
	}
	else if ("settings" == button_tracker->string.text)
	{
		current_page = PAGE::settings_menu;
	}
	else if ("back" == button_tracker->string.text)
	{
		current_page = PAGE::pause_menu;
	}
	else if ("how to play" == button_tracker->string.text)
	{

	}
	else if ("fullscreen" == button_tracker->string.text)
	{
		if (window->window_size == Window_size::fullscreen)
		{
			window->change_window_size(Window_size::high);
			window->window_size = Window_size::high;
		}
		else
		{
			window->change_window_size_fullscreen();
			window->window_size = Window_size::fullscreen;
		}
	}
	else if ("New game" == button_tracker->string.text)
	{
		current_page = PAGE::menu_level_select;
	}
	else if ("Quit" == button_tracker->string.text)
	{
		Message_Handler msg(MessageID::Event_Type::Quit);
		engine->Broadcast(&msg);
	}
	else if ("Level 1" == button_tracker->string.text)
	{
		current_page = PAGE::pause_menu;
	}
	else if ("Level 2" == button_tracker->string.text)
	{

	}
}


Menu::Menu()
	:pos{}, scale{}, texture_id{}
{
}

Menu::Menu(float x, float y, float scale_x, float scale_y, GLuint textre)
	:pos{Vec2(x,y)}, scale{Vec2(scale_x , scale_y)}, texture_id{ textre }
{
}

void draw_menu( Menu m)
{
	glBindTextureUnit(6, m.texture_id);
	glBindTexture(GL_TEXTURE_2D, m.texture_id);
	// load shader program in use by this object
	AssetManager::shaderval("image").Use();
	glBindVertexArray(AssetManager::modelval("square").vaoid);
	// copy object's model-to-NDC matrix to vertex shader's
	// uniform variable uModelToNDC
	Vec2 posM;
	int current_width;
	int current_height;
	glfwGetFramebufferSize(GLWindow::ptr_window, &current_width, &current_height);
	posM.x = m.pos.x * 2 / current_width ;
	posM.y = m.pos.y * 2 / current_height;
	Vec2 scale_mat;
	scale_mat.x = m.scale.x / current_width;
	scale_mat.y = m.scale.y / current_height;
	Mat3 mat = Mat3Translate(posM) * Mat3Scale(scale_mat) * Mat3RotRad(0.f);
	AssetManager::shaderval("image").SetUniform("uModel_to_NDC", mat.ToGlmMat3());
	// tell fragment shader sampler uTex2d will use texture image unit 6
	GLuint tex_loc = glGetUniformLocation(AssetManager::shaderval("image").GetHandle(), "uTex2d");
	glUniform1i(tex_loc, 6);
	// call glDrawElements with appropriate arguments
	glDrawElements(AssetManager::modelval("square").primitive_type, AssetManager::modelval("square").draw_cnt, GL_UNSIGNED_SHORT, 0);
	// unbind VAO and unload shader program
	glBindVertexArray(0);
	AssetManager::shaderval("image").UnUse();
}

void set_background()
{
	if (current_page == PAGE::pause_menu || current_page == PAGE::settings_menu )
	{
		menu_tracker = "Pause_menu";
	}
	else if (current_page == PAGE::main_menu || current_page == PAGE::menu_level_select)
	{
		menu_tracker = "Menu";
	}
	else
	{
		menu_tracker = "";
	}
}


void Buttons_update_resolution()
{
	int current_width, current_height;
	static int old_width{ window->width_init };
	static int old_height{ window->height_init };
	glfwGetFramebufferSize(GLWindow::ptr_window, &current_width, &current_height);
	if (old_width == current_width && old_height == current_height)
	{
		return;
	}
	float scaleX = static_cast<float>(current_width) / old_width;
	float scaleY = static_cast<float>(current_height) / old_height;
	old_width = current_width;
	old_height = current_height;
	input::update_resolution();

	for (int i = 0; i < total_page; ++i)
	{

		for (std::vector<Button>::iterator it = all_buttons.at(i).begin(); it != all_buttons.at(i).end(); ++it)
		{
			it->centre.x *= scaleX;
			it->centre.y *= scaleY;
			it->width *= scaleX;
			it->height *= scaleY;
			it->string.pos.x += scaleX;
			it->string.pos.y += scaleY;
		}
	}
		for (std::map<std::string,std::vector<Menu>>::iterator it = menu_background.begin(); it != menu_background.end(); ++it)
		{
			for (size_t i = 0; i < it->second.size(); ++i)
			{
				it->second.at(i).pos.x *= scaleX;
				it->second.at(i).pos.y *= scaleY;
				it->second.at(i).scale.x *= scaleX;
				it->second.at(i).scale.y *= scaleY;
			}
		}
}
