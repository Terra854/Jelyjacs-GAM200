#pragma once
#include <Precompile.h>
#include <ImGui/imgui.h>
#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_glfw.h>
#include <Imgui/imgui_impl_opengl3.h>
#include <input.h>
#include <GLWindow.h>

ImVec2 convertImGuiPosToWorldPos(ImVec2 pos);

ImVec2 convertMouseToGameViewportPos();