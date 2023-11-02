#pragma once
#include "Interface_System.h"
#include "Vec2.h"
#include <map>
#include "font.h"
#include "GLApp.h"
class GameHud
{
public:
	GameHud() = default;
	void Initialize() ;
	void Update() ;
	void Draw();
	~GameHud();
	
private:
};