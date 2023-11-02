#pragma once
#include "Interface_System.h"
#include "Vec2.h"
#include <map>

class GameHud
{
public:
	GameHud() = default;
	void Initialize() ;
	void Update() ;
	void Draw();
	~GameHud();
	class Text
	{
	public:
		Vec2 pos{};
		float scale;
		std::string text{};
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
	std::map<std::string,Button*> Buttons;
	void create_button(std::string const& text, Button button, float scale);
private:
};