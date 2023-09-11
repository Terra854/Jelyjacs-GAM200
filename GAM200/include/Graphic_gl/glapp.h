

/*                                                                      guard
----------------------------------------------------------------------------- */
#ifndef GLAPP_H
#define GLAPP_H

/*                                                                   includes
----------------------------------------------------------------------------- */
#include <GL/glew.h> // for access to OpenGL API declarations 
#include <GLFW/glfw3.h>
#include <glhelper.h>
#include <glslshader.h>

#include <iostream>
#include <list>


struct GLApp
{
	static void init ();
	static void update ();
	static void draw ();
	static void cleanup ();

	static void init_scene();

	// container for shader programs and helper function(s) ...
	struct GLModel
	{
		GLenum		primitive_type{ 0 };		// which OpenGL primitive to be rendered?
		GLuint		primitive_cnt{ 0 };			// added for tutorial 2
		GLuint		vaoid{ 0 };					// same as tutorial 1
		GLuint		draw_cnt{ 0 };				// added for tutorial 2
		GLSLShader	shdr_pgm;
		GLuint 	    texobj{ 0 };
		const char* texfile{ nullptr };
		
	};
	static GLModel mdl;
	static GLuint setup_texobj(const char*);
	static void insert_shdrpgm(std::string shdr_pgm_name, std::string vtx_shdr, std::string frg_shdr);
	struct GLViewport
	{
		GLint x , y;
		GLsizei width , height;
	};

	struct GLObject
	{
		glm::vec2 scaling{ 100.0f, 100.0f };
		float orientation;
		glm::vec2 position{ 0.0f, 0.0f };
		//glm::vec3 color{ 0 };
		glm::mat3 mdl_xform{ 0 };
		glm::mat3 mdl_to_ndc_xform{ 0 };

		std::map<std::string, GLApp::GLModel>::iterator mdl_ref;
		std::map<std::string, GLSLShader>::iterator shd_ref;
		std::map<std::string, GLuint>::iterator tex_ref;

		void update();
		void draw() const;
	};


	static std::map<std::string, GLSLShader> shdrpgms; // singleton
	static std::map<std::string, GLModel> models; // singleton
	static std::map<std::string, GLObject> objects; // singleton
	static std::map<std::string, GLuint> textures;
};

#endif /* GLAPP_H */
