/* !
@file    glapp.h
@author  Guo Chen (g.chen@digipen.edu)
@date    28/09/2023

Graphic system for the engine
includes all the functions to draw objects

*//*__________________________________________________________________________*/
/*                                                                  
----------------------------------------------------------------------------- */
#ifndef GLAPP_H
#define GLAPP_H
#include <Debug.h>

#include <GL/glew.h> // for access to OpenGL API declarations 
#include <GLFW/glfw3.h>
#include <glslshader.h>
#include <input.h>
#include <iostream>
#include <list>
#include <Interface_System.h>
#include <Factory.h>
#include <Camera.h>
#include <components/Texture.h>
#include <components/Transform.h>
#include <components/Physics.h>
#include <components/Body.h>
#include <Mat3.h>
#include <Vec2.h>
class GLApp : public ISystems
{
public:
	GLApp();
	~GLApp() {}
	virtual void Initialize() ;
	virtual void Update();
	virtual std::string SystemName() { return "Graphics"; }
	
	static void cleanup();

	//initial functions
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

	//helper function for loading png files
	static GLuint setup_texobj(const char*);
	//helper function set up shader program
	static void insert_shdrpgm(std::string shdr_pgm_name, std::string vtx_shdr, std::string frg_shdr);
	
	static void drawline(Vec2 start, Vec2 end);

	//helper function set up model
	// container for shader programs and models
	static std::map<std::string, GLSLShader> shdrpgms; 
	static std::map<std::string, GLModel> models;
	
	
	class Leveleditor {
	public:
		Leveleditor();
		Vec2 scale_window{ 800.0f,800.0f };
		void set_num(Vec2 num_) { num = num_; }
		void drawleveleditor();
		Vec2 num{0,0};
		float box_size{0};

	};
};
extern GLApp* app;
extern GLApp::Leveleditor* editor;
#endif  /*GLAPP_H */
