/*!
@file    GLWindow.h
@author  Guo Chen (g.chen@digipen.edu)
@date    28/09/2023

Window system for the engine
includes all the functions to create and update the window

*//*__________________________________________________________________________*/
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
	~GLWindow() {};
	virtual void Initialize();
	virtual void Update();
	virtual std::string SystemName() { return "Window"; };
	static void ActivateWindow();
	static void cleanup();

  // callbacks ...
  static void error_cb(int error, char const* description);
  static void fbsize_cb(GLFWwindow *ptr_win, int wwidth, int hheight);
 
  
  //static void update_time(double fpsCalcInt = 1.0);
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
