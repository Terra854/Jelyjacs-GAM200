/* !
@file	input.cpp
@author	Yeo Jia Ming
@date	28/9/2023

This file contains the implementations for input manager.
To detect mouse/keyboard key presses and mouse position.
*//*__________________________________________________________________________*/
#include <Debug.h>
#include "input.h"

//refer to input.h for input functions interface

namespace
{
	struct BUTTON
	{
	public:
		bool IsPressed();
		bool IsPressedRepeatedly();
		bool IsReleased();
		void SetKeyState(int action);
	private:
		bool pressed = false;
		bool released = true;
		bool pressedFirstTime = true;
	};

	struct MOUSE
	{
		double x;
		double y;
	};
	BUTTON buttons[static_cast<int>(KEY::total)]{};
	MOUSE mouse{};
}

bool BUTTON::IsPressed()
{
	if (pressed && pressedFirstTime)
	{
		pressedFirstTime = false;
		return true;
	}
	else return false;
}

bool BUTTON::IsReleased()
{
	return released;
}

bool BUTTON::IsPressedRepeatedly()
{
	return (pressed && !released);
}

void BUTTON::SetKeyState(int action)
{
	if (GLFW_PRESS == action)
	{
		pressed = true;
		released = false;
	}
	else if (GLFW_RELEASE == action)
	{
		released = true;
		pressed = false;
		pressedFirstTime = true;
	}
}

int at(KEY index)
{
	return static_cast<int>(index);
}

void KeyCallBack(GLFWwindow* pWin, int key, int scancode, int action, int mod)
{
	(void)pWin;
	(void)scancode;
	(void)mod;

	switch (key)
	{
	case GLFW_KEY_W:
	{
		buttons[at(KEY::w)].SetKeyState(action);
		break;
	}
	case GLFW_KEY_A:
	{
		buttons[at(KEY::a)].SetKeyState(action);
		break;
	}
	case GLFW_KEY_S:
	{
		buttons[at(KEY::s)].SetKeyState(action);
		break;
	}
	case GLFW_KEY_D:
	{
		buttons[at(KEY::d)].SetKeyState(action);
		break;
	}
	case GLFW_KEY_P:
	{
		buttons[at(KEY::p)].SetKeyState(action);
		break;
	}
	case GLFW_KEY_F:
	{
		buttons[at(KEY::f)].SetKeyState(action);
		break;
	}
	case GLFW_KEY_Z:
	{
		buttons[at(KEY::z)].SetKeyState(action);
		break;
	}
	case GLFW_KEY_X:
	{
		buttons[at(KEY::x)].SetKeyState(action);
		break;
	}
	case GLFW_KEY_L:
	{
		buttons[at(KEY::l)].SetKeyState(action);
		break;
	}
	case GLFW_KEY_UP:
	{
		buttons[at(KEY::up)].SetKeyState(action);
		break;
	}
	case GLFW_KEY_DOWN:
	{
		buttons[at(KEY::down)].SetKeyState(action);
		break;
	}
	case GLFW_KEY_LEFT:
	{
		buttons[at(KEY::left)].SetKeyState(action);
		break;
	}
	case GLFW_KEY_RIGHT:
	{
		buttons[at(KEY::right)].SetKeyState(action);
		break;
	}
	case GLFW_KEY_ESCAPE:
	{
		buttons[at(KEY::esc)].SetKeyState(action);
		break;
	}
	}
	/*
	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
	{
		glfwSetWindowShouldClose(pWin, GLFW_TRUE);
	}
	*/
}

void MouseButtonCallBack(GLFWwindow* pWin, int button, int action, int mod)
{
	(void)pWin;
	(void)mod;
	switch (button)
	{
	case GLFW_MOUSE_BUTTON_1:
	{
		buttons[at(KEY::mouseL)].SetKeyState(action);
	}
	break;
	case GLFW_MOUSE_BUTTON_2:
	{
		buttons[at(KEY::mouseR)].SetKeyState(action);
	}
	}
}

void MousePosCallBack(GLFWwindow* pWin, double xpos, double ypos)
{
	(void)pWin;
	mouse.x = xpos;
	mouse.y = ypos;
}

void input::Init(GLFWwindow* pWin)
{
	glfwSetKeyCallback(pWin, KeyCallBack);
	glfwSetMouseButtonCallback(pWin, MouseButtonCallBack);
	glfwSetCursorPosCallback(pWin, MousePosCallBack);
}


bool input::IsPressed(KEY key)
{
	return buttons[at(key)].IsPressed();
}


bool input::IsReleased(KEY key)
{
	return buttons[at(key)].IsReleased();
}


bool input::IsPressedRepeatedly(KEY key)
{
	return buttons[at(key)].IsPressedRepeatedly();
}

double input::GetMouseX()
{
	return mouse.x;
}
double input::GetMouseY()
{
	return mouse.y;
}
