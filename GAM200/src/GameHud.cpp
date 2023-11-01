#include "GameHud.h"
#include "Font.h"
#include "GLApp.h"
#include "input.h"


void GameHud::Initialize()
{
	create_button("button1", Button(Vec2(700 , 0),200 , 100) , 1);
	create_button("button2", Button(Vec2(700, -200), 200, 100), 1);
}
void GameHud::Update()
{
	
}

void GameHud::create_button(std::string const& text, Button button, float scale)
{
	button.string.pos.x -= find_width(text)/2 * scale ;
	button.string.pos.y -= 14 * scale;
	button.string.text = text;
	button.string.scale = scale;
	Buttons[text] = new Button{ button };
}

GameHud::Button::Button(Vec2 Pos1,Vec2 Pos2)
	:pos1{Pos1}, pos2{Pos2}
{
	string.pos.x = pos1.x + ((pos1.x + pos2.x) / 2);
	string.pos.y = pos1.y + ((pos1.y + pos2.y) / 2);
}

GameHud::Button::Button(Vec2 pos, float width, float height)
{
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
		GLApp::drawline(Vec2(ptr->pos1.x,ptr->pos1.y), Vec2(ptr->pos2.x , ptr->pos1.y), glm::vec3(1,1,1));
		GLApp::drawline(Vec2(ptr->pos1.x, ptr->pos2.y), Vec2(ptr->pos2.x, ptr->pos2.y), glm::vec3(1, 1, 1));
		GLApp::drawline(Vec2(ptr->pos1.x, ptr->pos1.y), Vec2(ptr->pos1.x, ptr->pos2.y), glm::vec3(1, 1, 1));
		GLApp::drawline(Vec2(ptr->pos2.x, ptr->pos1.y), Vec2(ptr->pos2.x, ptr->pos2.y), glm::vec3(1, 1, 1));

		DrawText(ptr->string.text, ptr->string.pos.x, ptr->string.pos.y, ptr->string.scale);
	}
}

