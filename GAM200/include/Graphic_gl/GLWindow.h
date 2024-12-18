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
enum class Window_size {
	fullscreen=0,
	high,
	medium,
	low,
	count
};
class GLWindow : public ISystems
  
{
public:
	
	GLWindow();
	~GLWindow() {};
	virtual void Initialize();
	virtual void Update();

	// Returns name of the component
	virtual std::string SystemName() { return "Window"; }

	static void ChangeWindowMode();
	static void window_iconify_callback(GLFWwindow* window, int iconified);
	static void cleanup();

  // callbacks ...
  static void error_cb(int error, char const* description);
  static void fbsize_cb(GLFWwindow *ptr_win, int wwidth, int hheight);
 
  static void change_window_size(Window_size size);
  static void change_window_size(int width, int height);
  static void change_window_size_fullscreen();

  //static void update_time(double fpsCalcInt = 1.0);
  static void print_specs();


  static GLint width, height;
  static GLint width_init, height_init;
  static GLint width_windowed, height_windowed;
  static GLdouble fps;
  static GLdouble delta_time; // time taken to complete most recent game loop
  static std::string title;
  static GLFWwindow *ptr_window;
  static Window_size window_size;
  std::string icon_path;
 

};

extern GLWindow* window;

#endif /* GLHELPER_H */
