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
	switch (key)
	{
	case GLFW_KEY_W:
	{
		buttons[at(KEY::w)].SetKeyState(action);
	}
	break;
	case GLFW_KEY_A:
	{
		buttons[at(KEY::a)].SetKeyState(action);
	}
	break;
	case GLFW_KEY_S:
	{
		buttons[at(KEY::s)].SetKeyState(action);
	}
	break;
	case GLFW_KEY_D:
	{
		buttons[at(KEY::d)].SetKeyState(action);
	}
	}
	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
	{
		glfwSetWindowShouldClose(pWin, GLFW_TRUE);
	}
}

void MouseButtonCallBack(GLFWwindow* pWin, int button, int action, int mod)
{
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


bool IsReleased(KEY key)
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
