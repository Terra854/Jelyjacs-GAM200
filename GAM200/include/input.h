#include "graphic.h"

enum class KEY
{
	w,
	a,
	s,
	d,
	mouseL,
	mouseR,
	total
};

namespace input
{
	/*------------------------------------------------------------------------------------------------------------*/
	/*!	returns true if key is triggered for the first time after released.
		If key is held down for more than 1 game loop, return false.
	*/
	bool IsPressed(KEY);
	/*------------------------------------------------------------------------------------------------------------*/
	/*!	returns true as long as key is held down for more than 1 game loop.
	*/
	bool IsPressedRepeatedly(KEY);
	/*------------------------------------------------------------------------------------------------------------*/
	/*! returns true if key is not pressed/pressedrepeatedly
	*/
	bool IsReleased(KEY);
	/*------------------------------------------------------------------------------------------------------------*/
	/*!	returns x position of mouse.
	*/
	double GetMouseX();
	/*------------------------------------------------------------------------------------------------------------*/
	/*!	returns y position of mouse.
	*/
	double GetMouseY();
	/*------------------------------------------------------------------------------------------------------------*/
	/*!	for use in graphics.cpp to initialise GLFW callbacks
	*/
	void Init(GLFWwindow* pWin);
}