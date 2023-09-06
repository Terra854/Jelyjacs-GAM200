
#ifndef Graphic_H
#define Graphic_H

/*                                                                   includes
----------------------------------------------------------------------------- */
#include <GL/glew.h> // for access to OpenGL API declarations 
#include <GLFW/glfw3.h>


#include <string>
#include <vector>

/*  _________________________________________________________________________ */
struct Graphic {
	static bool init(GLint w, GLint h, std::string t);
	static void update();
	static void draw();
	static void cleanup();
	static void setup_quad_vao();
	static void setup_shdrpgm();


	// geometry and material information ...
	static GLuint vaoid;        // with GL 4.5, VBO & EBO are not required
	static GLuint elem_cnt;     // how many indices in element buffer
	static GLuint pboid;        // id for PBO
	static GLuint texid;        // id for texture object
	//static GLSLShader shdr_pgm; // object that abstracts away nitty-gritty
	// details of shader management

	// callbacks ...
	static void error_cb(int error, char const* description);
	static void update_time(double fpsCalcInt = 1.0);

	//picture
	static void loadPicture();
	//static void RenderPNGImage(unsigned char* image_data, int width, int height);


	static GLint width, height;
	static GLdouble fps;
	static GLdouble delta_time; // time taken to complete most recent game loop
	static std::string title;
	static GLFWwindow* ptr_window;
};

#endif /* Graphic_H */
