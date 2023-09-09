
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h> // for access to OpenGL API declarations 
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <glapp.h>
#include <glhelper.h>
#include <array>
#include <vector>
#include <random>
#include <chrono>

/* Objects with file scope
----------------------------------------------------------------------------- */

//Declarations of shdrpgms models objects map
std::map<std::string , GLSLShader> GLApp::shdrpgms;
std::map<std::string , GLApp::GLModel> GLApp::models;
//std::map<std::string , GLApp::GLObject> GLApp::objects;

GLApp::GLModel GLApp::mdl{};

// Animation size 
GLuint images{ 0 };

glm::mat3 trans;


void GLApp::init ()
{
	// Part 1: Initialize OpenGL state ...
	glClearColor ( 1.f , 1.f , 1.f , 1.f );
	// Part 2: Use the entire window as viewport ...
	GLint w{ GLHelper::width } , h{ GLHelper::height };
	glViewport ( 0 , 0 , w , h );
	
	mdl.setup_vao();
	mdl.setup_shdrpgm();

	images = setup_texobj("../Assest/Picture/test3.png");
}


void GLApp::update ( )
{
	mdl.update();
}


void GLApp::draw ()
{
	std::stringstream ss;
	ss << std::fixed;

	ss << std::fixed;
	ss.precision(2);

	glfwSetWindowTitle(GLHelper::ptr_window, ss.str().c_str());

	// clear back buffer as before
	glClear(GL_COLOR_BUFFER_BIT);

	// now, render rectangular model from NDC coordinates to viewport
	mdl.draw();

}


void GLApp::cleanup ()
{
}


GLuint GLApp::setup_texobj(const char* pathname)
{
	int width{ }, height{  }, image_channels;
	
	unsigned char* ptr_texels = stbi_load(pathname, &width, &height, &image_channels, STBI_rgb_alpha);

	GLuint texobj_hdl;
	// define and initialize a handle to texture object that will
	// encapsulate two-dimensional textures
	glCreateTextures(GL_TEXTURE_2D, 1, &texobj_hdl);
	// allocate GPU storage for texture image data loaded from file
	glTextureStorage2D(texobj_hdl, 1, GL_RGBA8, width, height);
	// copy image data from client memory to GPU texture buffer memory
	glTextureSubImage2D(texobj_hdl, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, ptr_texels);
	// client memory not required since image is buffered in GPU memory
	delete[] ptr_texels;

	return texobj_hdl;
}

/*  _________________________________________________________________________ */
/*!  setup_vao

@param	none

@return none

Set up VAO for a square model
*/
void GLApp::GLModel::setup_vao()
{
	std::array <glm::vec2, 4 > pos_vtx
	{
		glm::vec2(1.0f, 1.0f) , glm::vec2(1.0f, -1.0f),
		glm::vec2(-1.0f, -1.0f) , glm::vec2(-1.0f, 1.0f)
	};
	std::array <glm::vec3, 4 > clr_vtx
	{
		glm::vec3(0.0f , 1.0f , 0.0f) , glm::vec3(0.0f , 0.0f , 1.0f),
		glm::vec3(1.0f , 0.0f , 1.0f) , glm::vec3(1.0f , 0.0f , 0.0f)
	};

	std::array <glm::vec2, 4 > tex_coor
	{
		glm::vec2(1.0f, 0.0f) , glm::vec2(1.0f, 1.0f),
		glm::vec2(0.0f, 1.0f) , glm::vec2(0.0f, 0.0f)
	};

	GLuint vbo;
	glCreateBuffers(1, &vbo);
	glNamedBufferStorage(vbo, sizeof(glm::vec2) * pos_vtx.size() + sizeof(glm::vec3) * clr_vtx.size() + sizeof(glm::vec2) * tex_coor.size(), nullptr, GL_DYNAMIC_STORAGE_BIT);
	glNamedBufferSubData(vbo, 0, sizeof(glm::vec2) * pos_vtx.size(), pos_vtx.data());
	glNamedBufferSubData(vbo, sizeof(glm::vec2) * pos_vtx.size(), sizeof(glm::vec3) * clr_vtx.size(), clr_vtx.data());
	glNamedBufferSubData(vbo, sizeof(glm::vec2) * pos_vtx.size() + sizeof(glm::vec3) * clr_vtx.size(),
		sizeof(glm::vec2) * tex_coor.size(), tex_coor.data());

	
	glCreateVertexArrays(1, &vaoid);

	glEnableVertexArrayAttrib(vaoid, 0);
	glVertexArrayVertexBuffer(vaoid, 0, vbo, 0, sizeof(glm::vec2));
	glVertexArrayAttribFormat(vaoid, 0, 2, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribBinding(vaoid, 0, 0);

	glEnableVertexArrayAttrib(vaoid, 1);
	glVertexArrayVertexBuffer(vaoid, 1, vbo, sizeof(glm::vec2) * pos_vtx.size(), sizeof(glm::vec3));
	glVertexArrayAttribFormat(vaoid, 1, 3, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribBinding(vaoid, 1, 1);

	glEnableVertexArrayAttrib(vaoid, 2);
	glVertexArrayVertexBuffer(vaoid, 2, vbo, sizeof(glm::vec2) * pos_vtx.size() + sizeof(glm::vec3) * clr_vtx.size(), sizeof(glm::vec2));
	glVertexArrayAttribFormat(vaoid, 2, 2, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribBinding(vaoid, 2, 2);

	std::array<GLushort, 6> idx_vtx
	{
		0, 1, 2,
		2, 3, 0
	};

	GLuint ebo_hdl;
	glCreateBuffers(1, &ebo_hdl);
	glNamedBufferStorage(ebo_hdl, sizeof(GLshort) * idx_vtx.size(), idx_vtx.data(), GL_DYNAMIC_STORAGE_BIT);
	glVertexArrayElementBuffer(vaoid, ebo_hdl);
	glBindVertexArray(0);

	
	primitive_type = GL_TRIANGLES;
	primitive_cnt = idx_vtx.size();
	draw_cnt = idx_vtx.size();
}
/*_________________________________________________________________________* /
/*! GLModel::setup_shdrpgm

@param	none

@return none

Read vert and frag file
*/
void GLApp::GLModel::setup_shdrpgm()
{
	std::vector<std::pair<GLenum, std::string>> shdr_files;
	shdr_files.push_back(std::make_pair(
		GL_VERTEX_SHADER,
		"../shaders/image.vert"));
	shdr_files.push_back(std::make_pair(
		GL_FRAGMENT_SHADER,
		"../shaders/image.frag"));
	shdr_pgm.CompileLinkValidate(shdr_files);
	if (GL_FALSE == shdr_pgm.IsLinked())
	{
		std::cout << "Unable to compile/link/validate shader programs" << "\n";
		std::cout << shdr_pgm.GetLog() << std::endl;
	}
}

/*_________________________________________________________________________* /
/*! GLModel::update

@param	none

@return none

Update based on input key T M A
*/

void GLApp::GLModel::update()
{
	trans = {
		{1.0f, 0.0f, 0.0f},
		{0.0f, 1.0f, 0.0f},
		{0.0f, 0.0f, 1.0f}
	};
	
}

/*_________________________________________________________________________* /
/*! GLModel::draw

@param	none

@return none

Draw model based on task, alpha blend, modulate
*/
void GLApp::GLModel::draw()
{
	glBindTextureUnit(6, images);

	
	// suppose shdrpgm is the handle to shader program object
	// that will render the rectangular model
	shdr_pgm.Use();
	glBindVertexArray(vaoid);
	// tell fragment shader sampler uTex2d will use texture image unit 6
	GLuint tex_loc = glGetUniformLocation(shdr_pgm.GetHandle(), "uTex2d");
	glUniform1i(tex_loc, 6);


	//unsigned int transformLoc = glGetUniformLocation(shdr_pgm.GetHandle(), "uModel_to_NDC");
	//glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

	//GLuint transformLoc = glGetUniformLocation(shdr_pgm.GetHandle(), "transform");
	//glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
	glDrawElements(primitive_type, draw_cnt, GL_UNSIGNED_SHORT, NULL);
	glBindVertexArray(0);
	shdr_pgm.UnUse();
}