/* !
@file Utils.h
@author Jonathan Woo
@date	11/11/2023

This file contains the declarations for the utility functions. These are meant
to help with internal functionality with the engine and not for overtly affecting
the engine itself or the game objects within it
*//*__________________________________________________________________________*/
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

//Used to check if a game object has been clicked on in the game scene
bool isObjectClicked(Transform* trans, ImVec2 mousePos);

//Calculates and return the angle between 2 Vec2 in degrees
float calculateAngle(const Vec2& a, const Vec2& b);