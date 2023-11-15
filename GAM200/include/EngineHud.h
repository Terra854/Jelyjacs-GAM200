#pragma once
/* !
@file	EngineHud.h
@author	Jonathan Woo Jia How
@date	02/11/2023

This file contains declarations for starting and stopping IMGui for the level editor
*//*__________________________________________________________________________*/
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