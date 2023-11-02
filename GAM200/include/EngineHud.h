/* !
@file    EngineHud.h
@author  Jonathan Woo Jia How
@date   30/10/2023

This file contains the declaration of class EngineHud that initializes all the 
necessary ImGui files in order for it to work in the game.
*//*__________________________________________________________________________*/

#pragma once
#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_glfw.h>
#include <Imgui/imgui_impl_opengl3.h>
#include <GLWindow.h>

class EngineHud
{
public:
	EngineHud() = default;
	~EngineHud();
	ImGuiIO& StartGui();
	void NewGuiFrame(bool showDemo = true);
	void GuiRender(ImGuiIO& io);
};