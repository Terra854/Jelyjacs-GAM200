#include "input.h"

//refer to input.h for input functions interface

namespace//anonymous namespace for internal linage
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

	BUTTON w{};
	BUTTON a{};
	BUTTON s{};
	BUTTON d{};
	BUTTON mouseL{};
	BUTTON mouseR{};
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

void KeyCallBack(GLFWwindow* pWin, int key, int scancode, int action, int mod)
{
	switch (key)
	{
	case GLFW_KEY_W:
	{
		w.SetKeyState(action);
	}
	break;
	case GLFW_KEY_A:
	{
		a.SetKeyState(action);

	}
	break;
	case GLFW_KEY_S:
	{
		s.SetKeyState(action);

	}
	break;
	case GLFW_KEY_D:
	{
		d.SetKeyState(action);

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
		mouseL.SetKeyState(action);
	}
	break;
	case GLFW_MOUSE_BUTTON_2:
	{
		mouseR.SetKeyState(action);
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

BUTTON& whichKey(KEY key)
{
	switch (key)
	{
	case KEY::w:
		return w;
	case KEY::a:
		return a;
	case KEY::s:
		return s;
	case KEY::d:
		return d;
	case KEY::mouseL:
		return  mouseL;
	case KEY::mouseR:
		return  mouseR;
	}
}

bool input::IsPressed(KEY key)
{
	return whichKey(key).IsPressed();
}


bool IsReleased(KEY key)
{
	return whichKey(key).IsReleased();
}


bool input::IsPressedRepeatedly(KEY key)
{
	return whichKey(key).IsPressedRepeatedly();
}

double input::GetMouseX()
{
	return mouse.x;
}
double input::GetMouseY()
{
	return mouse.y;
}
