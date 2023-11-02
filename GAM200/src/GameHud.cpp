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
		Text string{};
	};
	std::map<std::string, Button*> Buttons;


GLuint texture_id;

GLSLShader shdr_pgm;

}
void create_button(std::string const& text, Button button, float scale, FONT font);

void init_hud_assets();

void draw_hud_texture();
void GameHud::Initialize()
{
	init_hud_assets();
	create_button("pause", Button(Vec2(700, 0), 200, 100), 1, AldrichRegular);
	create_button("camera", Button(Vec2(700, -200), 200, 100), 1 , GeoRegular);
}

void GameHud::Update()
{
	if (input::IsPressed(KEY::mouseL))
	{
		for (auto it = Buttons.begin(); it != Buttons.end(); ++it)
		{
			Button* ptr = it->second;
			if (input::GetMouseX() < ptr->pos1.x || input::GetMouseX() > ptr->pos2.x || input::GetMouseY() < ptr->pos2.y || input::GetMouseY() > ptr->pos1.y)
			{
				continue;
			}
			std::cout << it->first << " is pressed" << std::endl;
			if (it->first == "pause")
			{
				engine->setPause();
			}
			else if (it->first == "camera")
			{
				if (camera2D->scale.x == 1.0f || camera2D->scale.y == 1.0f) {
					camera2D->scale = { 2.0f, 2.0f };
				}
				else camera2D->scale = { 1.0f, 1.0f };
			}
		}
	}
}

void create_button(std::string const& text, Button button, float scale , FONT font)
{
	button.string.pos.x -= find_width(text,font)/2 * scale ;
	button.string.pos.y -= 14 * scale;
	button.string.text = text;
	button.string.scale = scale;
	button.string.font = font;
	Buttons[text] = new Button{ button };
}

Button::Button(Vec2 Pos1,Vec2 Pos2)
	:pos1{Pos1}, pos2{Pos2}
{
	string.pos.x = pos1.x + ((pos1.x + pos2.x) / 2);
	string.pos.y = pos1.y + ((pos1.y + pos2.y) / 2);
}

Button::Button(Vec2 pos, float width, float height)
{
	string.pos = pos;
	pos1.x = pos.x - width / 2;
	pos2.x = pos.x + width / 2;
	pos1.y = pos.y + height / 2;
	pos2.y = pos.y - height / 2;
}

void GameHud::Draw()
{
	draw_hud_texture();
	for (auto it = Buttons.begin() ; it!=Buttons.end(); ++it)
	{
		Button* ptr = it->second;
		GLApp::drawline(Vec2(ptr->pos1.x,ptr->pos1.y), Vec2(ptr->pos2.x , ptr->pos1.y), glm::vec3(1,1,1));
		GLApp::drawline(Vec2(ptr->pos1.x, ptr->pos2.y), Vec2(ptr->pos2.x, ptr->pos2.y), glm::vec3(1, 1, 1));
		GLApp::drawline(Vec2(ptr->pos1.x, ptr->pos1.y), Vec2(ptr->pos1.x, ptr->pos2.y), glm::vec3(1, 1, 1));
		GLApp::drawline(Vec2(ptr->pos2.x, ptr->pos1.y), Vec2(ptr->pos2.x, ptr->pos2.y), glm::vec3(1, 1, 1));

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

void draw_hud_texture()
{
	glBindTextureUnit(6, texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	// load shader program in use by this object
	GLApp::shdrpgms["image"].Use();
	glBindVertexArray(GLApp::models["square"].vaoid);
	// copy object's model-to-NDC matrix to vertex shader's
	// uniform variable uModelToNDC
	Vec2 pos;
	pos.x = 0.f * 2 / window->width;
	pos.y = 0.f * 2 / window->height;
	Vec2 scale;
	scale.x = 500.f / window->width;
	scale.y = 100.f / window->height;
	Mat3 mat = Mat3Translate(pos) * Mat3Scale(scale)* Mat3RotRad(0.f);
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


