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
	float xDiff = (float) input::GetMouseX() - menuCentreWorldPos.x;
	float yDiff = (float) input::GetMouseY() - menuCentreWorldPos.y;
	xDiff *= (window->width / displaySize.x);
	yDiff *= (window->height / displaySize.y);
	output.x = xDiff;
	output.y = yDiff;
	return output;
}

bool isObjectClicked(Transform* trans, ImVec2 mousePos)
{
	Vec2 botLeft = trans->Position - (trans->Scale / 2);
	Vec2 topRight = trans->Position + (trans->Scale / 2);
	if (mousePos.x >= botLeft.x && mousePos.x <= topRight.x && mousePos.y >= botLeft.y && mousePos.y <= topRight.y)
	{
		return true;
	}
	else
	{
		return false;
	}
}