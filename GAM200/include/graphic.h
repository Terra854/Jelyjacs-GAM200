
#ifndef Graphic_H
#define Graphic_H

/*                                                                   includes
----------------------------------------------------------------------------- */
#include <GL/glew.h> // for access to OpenGL API declarations 
#include <GLFW/glfw3.h>
#include <string>

/*  _________________________________________________________________________ */
struct Graphic {
	static bool init(GLint w, GLint h, std::string t);
	static void update();
	static void draw();
	static void cleanup();

	// callbacks ...
	static void error_cb(int error, char const* description);
	static void fbsize_cb(GLFWwindow* ptr_win, int width, int height);
	// I/O callbacks ...
	static void key_cb(GLFWwindow* pwin, int key, int scancode, int action, int mod);
	static void mousebutton_cb(GLFWwindow* pwin, int button, int action, int mod);
	static void mousescroll_cb(GLFWwindow* pwin, double xoffset, double yoffset);
	static void mousepos_cb(GLFWwindow* pwin, double xpos, double ypos);

	static void update_time(double fpsCalcInt = 1.0);

	static GLint width, height;
	static GLdouble fps;
	static GLdouble delta_time; // time taken to complete most recent game loop
	static std::string title;
	static GLFWwindow* ptr_window;
};

#endif /* Graphic_H */
