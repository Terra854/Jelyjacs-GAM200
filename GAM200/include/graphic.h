
#ifndef Graphic_H
#define Graphic_H

/*                                                                   includes
----------------------------------------------------------------------------- */
#include <GL/glew.h> // for access to OpenGL API declarations 
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>

/*  _________________________________________________________________________ */
struct Graphic {
	// window information ...
	static GLint width, height;
	static GLdouble fps;
	static GLdouble delta_time; // time taken to complete most recent game loop
	static std::string title;
	static GLFWwindow* ptr_window;

	static bool init(GLint w, GLint h, std::string t);
	static void update();
	static void draw();
	static void cleanup();
	static void setup_quad_vao();
	static void setup_shdrpgm();

	static struct Image {
		const char* image_file;
		int image_width{}, image_height{}, image_channels{};
		unsigned char* image_data{};
	};
	

	static class Object {
		public:
		std::string object_name{};
		Image object_image;
		glm::vec2 position;
		glm::vec2 scale;
		GLfloat rotation;
		Object(Image object_image_input, glm::vec2 position_input, glm::vec2 scale_input, GLfloat rotation_input, std::string name) {
			this->object_image = object_image_input;
			this->position = position_input;
			this->scale = scale_input;
			this->rotation = rotation_input;
			this->object_name = name;
		}
	};
	//vector of objects
    static std::vector<Object> all_objects;

	//create object
	static void CreatObject(const char* file_name_input, glm::vec2 position_input, 
		glm::vec2 scale_input, GLfloat rotation_input, std::string name);

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

	
	//static void RenderPNGImage(unsigned char* image_data, int width, int height);

};

#endif /* Graphic_H */
