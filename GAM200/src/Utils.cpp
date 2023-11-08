#include "Utils.h"

ImVec2 convertImGuiPosToWorldPos(ImVec2 pos)
{
	ImVec2 output;
	output.x = pos.x - (window->width / 2);
	output.y = (window->height / 2) - pos.y;
	return output;
}

ImVec2 convertMouseToGameViewportPos(ImVec2 displaySize)
{
	ImVec2 output;
	ImVec2 menuProportions = ImGui::GetWindowSize();
	ImVec2 menuTopLeftPos = ImGui::GetWindowPos();
	ImVec2 menuImGuiCentrePos;
	menuImGuiCentrePos.x = menuTopLeftPos.x + (menuProportions.x / 2);
	menuImGuiCentrePos.y = menuTopLeftPos.y + (menuProportions.y / 2);
	ImVec2 menuCentreWorldPos = convertImGuiPosToWorldPos(menuImGuiCentrePos);
	float xDiff = input::GetMouseX() - menuCentreWorldPos.x;
	float yDiff = input::GetMouseY() - menuCentreWorldPos.y;
	xDiff *= (window->width / displaySize.x);
	yDiff *= (window->height / displaySize.y);
	output.x = xDiff;
	output.y = yDiff;
	return output;
}