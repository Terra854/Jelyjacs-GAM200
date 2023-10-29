#include "GameHud.h"
#include "Font.h"
#include "GLApp.h"

GameHud* gamehud = NULL;
GameHud::GameHud() {
}

std::vector<GameHud::Button> Buttons;
int descent = 11;


void GameHud::Initialize()
{
	create_button("button1", Vec2(0, -500) , 1);
}

void GameHud::Update()
{

}

void GameHud::Draw()
{
		for (Button b : Buttons)
		{
			DrawText(b.text, b.pos1.x, b.pos1.y + b.scale * descent, b.scale);
			GLApp::drawline(b.pos1, Vec2(b.pos2.x, b.pos1.y));
			GLApp::drawline(Vec2(b.pos1.x, b.pos2.y), b.pos2);
			GLApp::drawline(b.pos1, Vec2(b.pos1.x, b.pos2.y));
			GLApp::drawline(Vec2(b.pos2.x, b.pos1.y), b.pos2);
		}
}


void GameHud::create_button(std::string const& str, Vec2 pos, float scale)
{
	float x = (find_width(str)) * scale;
	float y = (48+ descent/2) * scale;
	Button b = { str, pos, Vec2(pos.x + x, pos.y + y)  , scale};
	Buttons.push_back(b);
}

