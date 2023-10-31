#include "GameHud.h"
#include "Font.h"
#include "GLApp.h"
#include "input.h"

std::vector<GameHud::Button> Buttons;
int descent = 11;

void GameHud::Initialize()
{
	create_button("button1g", Vec2(500, 0), 2);
}
void GameHud::Update()
{
	if (input::IsPressedRepeatedly(KEY::mouseL))
	{
		for (Button b : Buttons)
		{
			if (input::GetMouseX() < b.pos1.x || input::GetMouseX() > b.pos2.x)
			{
				return;
			}
			if (input::GetMouseY() > b.pos1.y || input::GetMouseY() < b.pos2.y)
			{
				return;
			}
			std::cout << b.text << "Is pressed!!" << std::endl;
		}
	}
}
void GameHud::Draw()
{
	for (Button b : Buttons)
	{
		DrawText(b.text, b.pos1.x, b.pos1.y - b.scale * (48-descent), b.scale);
		GLApp::drawline(b.pos1, Vec2(b.pos2.x, b.pos1.y));
		GLApp::drawline(Vec2(b.pos1.x, b.pos2.y), b.pos2);
		GLApp::drawline(b.pos1, Vec2(b.pos1.x, b.pos2.y));
		GLApp::drawline(Vec2(b.pos2.x, b.pos1.y), b.pos2);
	}
}
void GameHud::create_button(std::string const& str, Vec2 pos, float scale)
{
	float x = (find_width(str)) * scale;
	float y = 48 * scale;
	Button b = { str, pos, Vec2(pos.x + x, pos.y - y)  , scale };
	Buttons.push_back(b);
}
