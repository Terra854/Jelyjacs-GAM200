/* !
@file	input.cpp
@author	Yeo Jia Ming
@date	28/9/2023

This file contains the implementations for input manager.
To detect mouse/keyboard key presses and mouse position.
*//*__________________________________________________________________________*/
#include <Precompile.h>
#include <Debug.h>
#include "input.h"
#include "GLWindow.h"
#include "Core_Engine.h"
//refer to input.h for input functions interface

namespace
{
	GLFWwindow* Pwindow = nullptr;
	float scaleX{ 1.0f };
	double scaleY{ 1.0f };
	int new_width;
	int new_height;
	float old_width;
	float old_height;
	struct BUTTON
	{
	public:
		bool IsPressed();
		bool IsPressedRepeatedly();
		bool IsReleased();
		void SetKeyState(int action);
	    float timer = 0.0f;
	private:
		bool pressed = false;
		bool released = true;
		bool pressedPrevFrame = false;
	friend void input::Update();
	};

	struct MOUSE
	{
		double x;
		double y;
		bool moved = false;
	};
	BUTTON buttons[static_cast<int>(KEY::total)]{};
	MOUSE mouse{};
}

bool BUTTON::IsPressed()
{
	return (pressed && !pressedPrevFrame);
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
		pressedPrevFrame = false;
		timer = 0;
	}
}

int at(KEY index)
{
	return static_cast<int>(index);
}


void MousePosCallBack(GLFWwindow* pWin, double xpos, double ypos)
{
	(void)pWin;
	mouse.moved = true;
	mouse.x = xpos;
	mouse.y = ypos;
}

void input::Init(GLFWwindow* pWin)
{
	Pwindow = pWin;
	glfwSetCursorPosCallback(pWin, MousePosCallBack);
	old_width = window->width;
	old_height = window->height;
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
		return ((mouse.x) - static_cast<double>(old_width*scaleX / 2.0f));
}
double input::GetMouseY()
{
		return (-mouse.y) + (old_height*scaleY/ 2.0f);
}

bool mouse_update()
{
	static float mouse_prevX = 0.0f;
	static float mouse_prevY = 0.0f;
	if (mouse.x == mouse_prevX && mouse.y == mouse_prevY)
	{
		return false;
	}
	else
	{
		mouse_prevX = (float) mouse.x;
		mouse_prevY = (float) mouse.y;
		return true;
	}

}

void input::Update()
{
	for (int i = 0; i < at(KEY::total); ++i)
	{
		if (buttons[i].pressed && !buttons[i].pressedPrevFrame)
		{
			buttons[i].pressedPrevFrame = true;
		}
	}
	mouse.moved = mouse_update();
	buttons[at(KEY::w)].SetKeyState(glfwGetKey(Pwindow, GLFW_KEY_W));
	buttons[at(KEY::a)].SetKeyState(glfwGetKey(Pwindow, GLFW_KEY_A));
	buttons[at(KEY::s)].SetKeyState(glfwGetKey(Pwindow, GLFW_KEY_S));
	buttons[at(KEY::d)].SetKeyState(glfwGetKey(Pwindow, GLFW_KEY_D));
	buttons[at(KEY::l)].SetKeyState(glfwGetKey(Pwindow, GLFW_KEY_L));
	buttons[at(KEY::p)].SetKeyState(glfwGetKey(Pwindow, GLFW_KEY_P));
	buttons[at(KEY::f)].SetKeyState(glfwGetKey(Pwindow, GLFW_KEY_F));
	buttons[at(KEY::z)].SetKeyState(glfwGetKey(Pwindow, GLFW_KEY_Z));
	buttons[at(KEY::x)].SetKeyState(glfwGetKey(Pwindow, GLFW_KEY_X));
	buttons[at(KEY::c)].SetKeyState(glfwGetKey(Pwindow, GLFW_KEY_C));
	buttons[at(KEY::mouseL)].SetKeyState(glfwGetMouseButton(Pwindow, GLFW_MOUSE_BUTTON_1));
	buttons[at(KEY::mouseR)].SetKeyState(glfwGetMouseButton(Pwindow, GLFW_MOUSE_BUTTON_2));
	buttons[at(KEY::up)].SetKeyState(glfwGetKey(Pwindow, GLFW_KEY_UP));
	buttons[at(KEY::down)].SetKeyState(glfwGetKey(Pwindow, GLFW_KEY_DOWN));
	buttons[at(KEY::left)].SetKeyState(glfwGetKey(Pwindow, GLFW_KEY_LEFT));
	buttons[at(KEY::right)].SetKeyState(glfwGetKey(Pwindow, GLFW_KEY_RIGHT));
	buttons[at(KEY::esc)].SetKeyState(glfwGetKey(Pwindow, GLFW_KEY_ESCAPE));
	buttons[at(KEY::q)].SetKeyState(glfwGetKey(Pwindow, GLFW_KEY_Q));
	buttons[at(KEY::e)].SetKeyState(glfwGetKey(Pwindow, GLFW_KEY_E));
	buttons[at(KEY::tab)].SetKeyState(glfwGetKey(Pwindow, GLFW_KEY_TAB));
	buttons[at(KEY::enter)].SetKeyState(glfwGetKey(Pwindow, GLFW_KEY_ENTER));
	buttons[at(KEY::del)].SetKeyState(glfwGetKey(Pwindow, GLFW_KEY_DELETE));
	buttons[at(KEY::spacebar)].SetKeyState(glfwGetKey(Pwindow, GLFW_KEY_SPACE));
	buttons[at(KEY::shift)].SetKeyState(glfwGetKey(Pwindow, GLFW_KEY_LEFT_SHIFT));
	buttons[at(KEY::one)].SetKeyState(glfwGetKey(Pwindow, GLFW_KEY_1));
	buttons[at(KEY::two)].SetKeyState(glfwGetKey(Pwindow, GLFW_KEY_2));
}

bool input::IsPressedRepeatedlyDelayed(KEY key , float delay)
{
	if (buttons[at(key)].IsPressedRepeatedly())
	{
		buttons[at(key)].timer += engine->GetDt();
		if (buttons[at(key)].timer > delay)
		{
			return true;
		}
	}
	return false;
}

bool input::MouseMoved()
{
	return mouse.moved;
}

void input::update_resolution()
{
	//new_width = glfwGetVideoMode(glfwGetPrimaryMonitor())->width;
	//new_height = glfwGetVideoMode(glfwGetPrimaryMonitor())->height;
	glfwGetFramebufferSize(GLWindow::ptr_window, &new_width, &new_height);
	scaleX = new_width / old_width;
	scaleY = new_height / old_height;

}
