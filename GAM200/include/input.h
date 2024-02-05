#pragma once
/* !
@file	input.h
@author	Yeo Jia Ming, Luke Goh
@date	27/11/2023

This file contains declarations for the input manager functions.
To detect mouse/keyboard key presses and mouse position.
*//*__________________________________________________________________________*/
#include <Debug.h>
#include <message.h>
#include <GLFW/glfw3.h>

//list of all keys
enum class KEY
{
	w,
	a,
	s,
	d,
	q,
	e,
	t,
	l,
	p,
	f,
	z,
	x,
	c,
	k,
	mouseL,
	mouseR,
	up,
	down,
	left,
	right,
	esc,
	tab,
	enter,
	del,
	spacebar,
	shift,
	one,
	two,
	total
};


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
	
	//to initialise input system
	void Init(GLFWwindow* pWin);

	//updates each key state
	void Update();

	//checks if mouse moved
	bool MouseMoved();

	//updates input scaling if resolution changed
	void update_resolution();
}