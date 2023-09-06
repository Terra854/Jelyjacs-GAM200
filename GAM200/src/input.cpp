#include "input.h"



KEY input::w{};
KEY input::a{};
KEY input::s{};
KEY input::d{};
KEY input::mouseL{};
KEY input::mouseR{};
MOUSE input::mouse{};

bool KEY::IsPressed()
{
	if (pressed && !pressedPrevFrame)
	{
		pressedPrevFrame = true;
		return true;
	}
	else return false;
}

bool KEY::IsPressedRepeatedly()
{
	return (pressed && !released);
}

void MOUSE::SetMousePos(int x , int y)
{
	MOUSE::x = x;
	MOUSE::y = y;
}


void KEY::SetKeyState(int action)
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
		pressedPrevFrame = false;
	}
}


void input::KeyCallBack(GLFWwindow* pwin, int key, int scancode, int action, int mod)
{
	switch (key)
	{
	case GLFW_KEY_W:
	{
		input::w.SetKeyState(action);
	}
	break;
	case GLFW_KEY_A:
	{
		input::a.SetKeyState(action);

	}
	break;
	case GLFW_KEY_S:
	{
		input::s.SetKeyState(action);

	}
	break;
	case GLFW_KEY_D:
	{
		input::d.SetKeyState(action);

	}
	}
	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
	{
		glfwSetWindowShouldClose(pwin, GLFW_TRUE);
	}
}

int MOUSE::GetPosX()
{
	return x;
}

int MOUSE::GetPosY()
{
	return y;
}
void input::MouseButtonCallBack(GLFWwindow* pwin, int button, int action, int mod)
{
	switch (button)
	{
	case GLFW_MOUSE_BUTTON_1:
	{
		input::mouseL.SetKeyState(action);

	}
	break;
	case GLFW_MOUSE_BUTTON_2:
	{
		input::mouseR.SetKeyState(action);
	}
	}
}

void input::MousePosCallBack(GLFWwindow* pwin, double xpos, double ypos)
{
	mouse.SetMousePos(xpos, ypos);
}

void input::init(GLFWwindow* pwin)
{
	glfwSetKeyCallback(pwin, input::KeyCallBack);
	glfwSetMouseButtonCallback(pwin, input::MouseButtonCallBack);
	glfwSetCursorPosCallback(pwin, input::MousePosCallBack);
}
