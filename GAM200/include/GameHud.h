/* !
@file GameHud.h
@author Yeo Jia Ming
@date	3/11/2023

This file contains the declarations for the functions of gamehud class
*//*__________________________________________________________________________*/
#pragma once
/* !
@file	GameHud.h
@author	Yeo Jia Ming
@date	02/11/2023

This file contains declarations for the in-game hud
*//*__________________________________________________________________________*/
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