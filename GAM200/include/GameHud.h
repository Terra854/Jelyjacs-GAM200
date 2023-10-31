#pragma once
#include "Interface_System.h"
#include "Vec2.h"
#include "vector"
class GameHud
{
public:
	GameHud() = default;
	virtual void Initialize() ;
	virtual void Update() ;
	void Draw();
	class Button
	{
	public:
		std::string text;
		Vec2 pos1{};
		Vec2 pos2{};
		float scale;
	};
	void create_button(std::string const&, Vec2, float);
private:
};