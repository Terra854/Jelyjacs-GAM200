/* !
@file Sandbox.cpp
@author Luke Goh
@date	3/11/2023

This file contains the class that is used to run the game
*//*__________________________________________________________________________*/
#include <GAM200.h>

class Sandbox : public Application 
{
public:
	Sandbox()
	{

	}

	~Sandbox()
	{

	}
};

Application* CreateApplication() 
{
	return new Sandbox();		// This is to create a new instance for the game
}
