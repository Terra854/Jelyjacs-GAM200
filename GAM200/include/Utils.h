#pragma once
#include <Precompile.h>
#include <ImGui/imgui.h>
#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_glfw.h>
#include <Imgui/imgui_impl_opengl3.h>
#include <input.h>
#include <GLWindow.h>
#include <iostream>
#include <components/Transform.h>

//Takes an ImVec2 and converts it as though it were a position relative to the ImGui library
//i.e. The top left of the screen is (0, 0) and the y axis goes downwards and the x-axis goes to the right
ImVec2 convertImGuiPosToWorldPos(ImVec2 pos);

//Converts the mouse position to a position relative to the game scene in the viewport
//Primarily for use with the Transform of game objects
//Needs the display size of the game scene as a parameter
ImVec2 convertMouseToGameViewportPos(ImVec2 displaySize);

bool isObjectClicked(Transform* trans, ImVec2 mousePos);