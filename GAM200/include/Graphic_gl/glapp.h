#ifndef GLAPP_H
#define GLAPP_H

/*                                                                  
----------------------------------------------------------------------------- */
#include <Debug.h>

#include <GL/glew.h> // for access to OpenGL API declarations 
#include <GLFW/glfw3.h>
#include <glslshader.h>
#include <input.h>
#include <iostream>
#include <list>
#include <Interface_System.h>
#include <Factory.h>
#include <components/Texture.h>
#include <components/Transform.h>
#include <components/Physics.h>
#include <components/Body.h>
class GLApp : public ISystems
{
public:
	GLApp();
	~GLApp() {}
	virtual void Initialize() ;
	virtual void Update(float time);
	virtual std::string SystemName() { return "Graphics"; }
	
	static void cleanup();

	static void init_models();
	static void init_shdrpgms();

	// container for shader programs and helper function(s) ...
	struct GLModel
	{
		GLenum		primitive_type{ 0 };		
		GLuint		primitive_cnt{  };			
		GLuint		vaoid{ 0 };					
		GLuint		draw_cnt{ 0 };				
		GLSLShader	shdr_pgm;
		GLuint 	    texobj{ 0 };
		const char* texfile{ nullptr };

	};
	
	static GLuint setup_texobj(const char*);
	static void insert_shdrpgm(std::string shdr_pgm_name, std::string vtx_shdr, std::string frg_shdr);
	struct GLViewport
	{
		GLint x, y;
		GLsizei width, height;
	};



	static std::map<std::string, GLSLShader> shdrpgms; 
	static std::map<std::string, GLModel> models;
	
};
extern GLApp* app;
#endif /* GLAPP_H */
