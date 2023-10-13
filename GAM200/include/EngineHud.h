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