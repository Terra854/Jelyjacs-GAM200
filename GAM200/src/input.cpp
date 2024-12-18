/* !
@file	input.cpp
@author	Yeo Jia Ming
@date	27/11/2023

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
	
	//keep track of resolution change
	float scaleX{ 1.0f };
	double scaleY{ 1.0f };
	int new_width;
	int new_height;
	int old_width;
	int old_height;

	bool mouse_is_clicked_once;

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

void mouse_button_callback(GLFWwindow*, int button, int action, int )
{
	if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_RELEASE)
	{
		mouse_is_clicked_once = true;
	}
}

void input::Init(GLFWwindow* pWin)
{
	Pwindow = pWin;
	glfwSetCursorPosCallback(pWin, MousePosCallBack);
	glfwGetWindowSize(GLWindow::ptr_window, &old_width, &old_height);
	new_width = old_width;
	new_height = old_height;
	glfwSetMouseButtonCallback(pWin, mouse_button_callback);
}


bool input::MouseClickedOnce()
{
	if (mouse_is_clicked_once && input::IsPressed(KEY::mouseL))
	{
		mouse_is_clicked_once = false;
		return true;
	}
	else return false;
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

float input::GetMouseX()
{
	return (static_cast<float>(mouse.x) - static_cast<float>(new_width) / 2.0f) * scaleX;
}
float input::GetMouseY()
{
	return (static_cast<float>(-mouse.y) + static_cast<float>(new_height) / 2.0f) * static_cast<float>(scaleY);
}

//checks if mouse was moved
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

bool input::LevelEditorTextActive = false;

void input::Update()
{
	//If any text box in the level editor is active
	//Do not register any inputs. ImGui uses it's own input system seperate from this
	if (LevelEditorTextActive)
		return;

	//updates each key if it was pressed previous frame
	for (int i = 0; i < at(KEY::total); ++i)
	{
		if (buttons[i].pressed && !buttons[i].pressedPrevFrame)
		{
			buttons[i].pressedPrevFrame = true;
		}
	}

	//set the updated key states into the vector
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
	buttons[at(KEY::k)].SetKeyState(glfwGetKey(Pwindow, GLFW_KEY_K));
	buttons[at(KEY::mouseL)].SetKeyState(glfwGetMouseButton(Pwindow, GLFW_MOUSE_BUTTON_1));
	buttons[at(KEY::mouseR)].SetKeyState(glfwGetMouseButton(Pwindow, GLFW_MOUSE_BUTTON_2));
	buttons[at(KEY::up)].SetKeyState(glfwGetKey(Pwindow, GLFW_KEY_UP));
	buttons[at(KEY::down)].SetKeyState(glfwGetKey(Pwindow, GLFW_KEY_DOWN));
	buttons[at(KEY::left)].SetKeyState(glfwGetKey(Pwindow, GLFW_KEY_LEFT));
	buttons[at(KEY::right)].SetKeyState(glfwGetKey(Pwindow, GLFW_KEY_RIGHT));
	buttons[at(KEY::esc)].SetKeyState(glfwGetKey(Pwindow, GLFW_KEY_ESCAPE));
	buttons[at(KEY::q)].SetKeyState(glfwGetKey(Pwindow, GLFW_KEY_Q));
	buttons[at(KEY::e)].SetKeyState(glfwGetKey(Pwindow, GLFW_KEY_E));
	buttons[at(KEY::t)].SetKeyState(glfwGetKey(Pwindow, GLFW_KEY_T));
	buttons[at(KEY::tab)].SetKeyState(glfwGetKey(Pwindow, GLFW_KEY_TAB));
	buttons[at(KEY::enter)].SetKeyState(glfwGetKey(Pwindow, GLFW_KEY_ENTER));
	buttons[at(KEY::del)].SetKeyState(glfwGetKey(Pwindow, GLFW_KEY_DELETE));
	buttons[at(KEY::spacebar)].SetKeyState(glfwGetKey(Pwindow, GLFW_KEY_SPACE));
	buttons[at(KEY::shift)].SetKeyState(glfwGetKey(Pwindow, GLFW_KEY_LEFT_SHIFT));
	buttons[at(KEY::one)].SetKeyState(glfwGetKey(Pwindow, GLFW_KEY_1));
	buttons[at(KEY::two)].SetKeyState(glfwGetKey(Pwindow, GLFW_KEY_2));


	input::update_resolution();
}

//sets a delay when press repeatedly
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

//updates scaling of inputs based on resolution change
void input::update_resolution()
{
	glfwGetWindowSize(GLWindow::ptr_window, &new_width, &new_height);
	scaleX =  static_cast<float>(old_width) / static_cast<float>(new_width);
	scaleY = static_cast<float>(old_height) / static_cast<float>(new_height);

}
