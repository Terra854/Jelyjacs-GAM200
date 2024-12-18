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
#include <Core_Engine.h>
#include <Mat3.h>
#include <Vec2.h>
class GLApp : public ISystems
{
public:
	GLApp();
	~GLApp() {
		cleanup();
	}
	virtual void Initialize() ;
	virtual void Update();

	// Returns name of the component
	virtual std::string SystemName() { return "Graphics"; }
	
	static void cleanup();

	//initial functions
	static void init_models();
	static void insert_models(std::string);
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

	static void drawline(Vec2 start, Vec2 end, glm::vec3 color);
	static void drawtriangle(Vec2 position, Vec2 Scale, float rotation, glm::vec3 color);
	static void drawline_circle(Vec2 position, Vec2 Scale, float kuandu, glm::vec3 color);
	static void draw_rect(Vec2 position, Vec2 Scale, float rotation, glm::vec3 color);

	static void draw_texture( Vec2 tex_t, Vec2 tex_s, float tex_r, GLuint tex_in, bool tex_camera);
	
	bool video_start = false;
	float video_timer = 0.0f;
	int video_count = 1;

	static Vec2 game_to_ndc(Vec2 position);
	static float game_to_ndc(float position);
};
extern GLApp* app;
#endif  /*GLAPP_H */
