#include "GameHud.h"
#include "Font.h"
#include "GLApp.h"
#include "input.h"


void GameHud::Initialize()
{
	create_button("button1", Button(Vec2(500 , 0),100 , 50) , 1);
}
void GameHud::Update()
{

}

void GameHud::create_button(std::string const& text, Button button, float scale)
{
	button.string.text = text;
	button.string.scale = scale;
	Buttons[text] = new Button{ button };
}

GameHud::Button::Button(Vec2 Pos1,Vec2 Pos2)
	:pos1{Pos1}, pos2{Pos2}
{
	string.centre.x = pos1.x + ((pos1.x + pos2.x) / 2);
	string.centre.y = pos1.y + ((pos1.y + pos2.y) / 2);
}

GameHud::Button::Button(Vec2 pos, float width, float height)
{
	string.centre = pos;
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
		DrawText(ptr->string.text, ptr->string.centre.x, ptr->string.centre.y, ptr->string.scale);
	}
}

