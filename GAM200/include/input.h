#pragma once
/* !
@file	input.h
@author	Yeo Jia Ming, Luke Goh
@date	28/9/2023

This file contains declarations for the input manager functions.
To detect mouse/keyboard key presses and mouse position.
*//*__________________________________________________________________________*/
#include <Debug.h>
#include <message.h>
#include <GLFW/glfw3.h>

enum class KEY
{
	w,
	a,
	s,
	d,
	l,
	p,
	f,
	z,
	x,
	c,
	mouseL,
	mouseR,
	up,
	down,
	left,
	right,
	esc,
	total
};
/*
class EscapeExit : public Message_Handler {
	EscapeExit() : Message_Handler(MessageID::Quit) {}
};
*/

enum Movement_direction {
	none = 0,
	up,
	down,
	left,
	right
};
class MovementKey : public Message_Handler {
public:
	MovementKey() : Message_Handler(MessageID::Movement), dir(none) {}
	MovementKey(Movement_direction key) : Message_Handler(MessageID::Movement), dir(key) {}
	Movement_direction dir;
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

	bool IsPressedRepeatedlyDelayed(KEY , float delay=1.0f);

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
	void Update();
}