/*!
@file    GLWindow.h
@author  pghali@digipen.edu, g.chen@digipen.edu
@date    10/06/2023

This file contains the declaration of namespace Helper that encapsulates the
functionality required to create an OpenGL context using GLFW; use GLEW
to load OpenGL extensions; initialize OpenGL state; and finally initialize
the OpenGL application by calling initalization functions associated with
objects participating in the application.

*//*__________________________________________________________________________*/

/*                                                                      guard
----------------------------------------------------------------------------- */
#ifndef GLHELPER_H
#define GLHELPER_H

/*                                                                   includes
----------------------------------------------------------------------------- */
#include <Debug.h>

#include <GL/glew.h> // for access to OpenGL API declarations 
#include <GLFW/glfw3.h>
#include <string>
#include <Interface_System.h>


/*  _________________________________________________________________________ */
class GLWindow : public ISystems
  
{
public:
	GLWindow();
	virtual void Initialize();
	virtual void Update(float time);
	virtual std::string GetSystemName() { return "Window"; };
	static void ActivateWindow();
	static void cleanup();

  // callbacks ...
  static void error_cb(int error, char const* description);
  static void fbsize_cb(GLFWwindow *ptr_win, int wwidth, int hheight);
  // I/O callbacks ...
  
  static void update_time(double fpsCalcInt = 1.0);
  static void print_specs();


  static GLint width, height;
  static GLdouble fps;
  static GLdouble delta_time; // time taken to complete most recent game loop
  static std::string title;
  static GLFWwindow *ptr_window;
};

enum class Gamestate {
	start,
	end
};


extern GLWindow* window;

#endif /* GLHELPER_H */
