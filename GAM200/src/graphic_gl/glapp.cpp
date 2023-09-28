/* !
@file    glapp.cpp
@author  Guo Chen (g.chen@digipen.edu)
@date    28/09/2023

Graphic system for the engine
includes all the functions to draw objects

*//*__________________________________________________________________________*/
#include <Debug.h>
#include <cmath>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h> // for access to OpenGL API declarations 
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <glapp.h>
#include <GLWindow.h>
#include <array>
#include <vector>
#include <random>
#include <chrono>
#include <components/PlayerControllable.h>

/* Objects with file scope
----------------------------------------------------------------------------- */
//debug 
bool graphics_debug{ false };//press p to activate debug mode
glm::vec3 box_color{ 0.0f, 0.0f, 1.0f };
glm::vec3 line_color{ 0.0f, 1.0f, 0.0f };


//Declarations of shdrpgms models objects map
std::map<std::string, GLSLShader> GLApp::shdrpgms;
std::map<std::string, GLApp::GLModel> GLApp::models;

//Global pointer to GLApp
GLApp* app = NULL;
GLApp::GLApp()
{
	app = this;
}

/*
* Initialize() is called once, at program start.
*/
void GLApp::Initialize()
{
	glClearColor(1.f, 1.f, 1.f, 1.f);

	glViewport(0, 0, window->width, window->height);
	init_models();
	init_shdrpgms();
	
	// enable alpha blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

/*
* get all he models from the list.txt file and store them in the models map
*/
void GLApp::init_models() {
	//open list of meshes
	std::ifstream ifs{ "../meshes/list.txt", std::ios::in };
	if (!ifs)
	{
		std::cout << "ERROR: Unable to open mesh file: "
			<< "list" << "\n";
		exit(EXIT_FAILURE);
	}
	ifs.seekg(0, std::ios::beg);
	std::string line;
	//get models from list
	while (getline(ifs, line)) {
		//get model name
		std::istringstream line_model_name{ line };
		std::string model_name;
		line_model_name >> model_name;

		//check if model already exists
		if (models.find(model_name) == models.end()) {
			GLModel Model;
			std::ifstream ifs_msh{ "../meshes/" + model_name + ".msh", std::ios::in };
			if (!ifs_msh)
			{
				std::cout << "ERROR: Unable to open mesh file: "
					<< model_name << "\n";
				exit(EXIT_FAILURE);
			}
			ifs_msh.seekg(0, std::ios::beg);
			std::string line_mesh;
			getline(ifs_msh, line_mesh);
			std::istringstream line_sstm_mesh{ line_mesh };
			char obj_prefix;
			std::string mesh_name;
			line_sstm_mesh >> obj_prefix >> mesh_name;

			//get model data
			std::vector < float > pos_vtx;
			std::vector < float > clr_vtx;
			std::vector < float > tex_coor;
			std::vector < GLushort > gl_tri_primitives;

			GLuint vbo, vao, ebo;

			while (getline(ifs_msh, line_mesh))
			{
				std::istringstream line_sstm_mdl{ line_mesh };
				line_sstm_mdl >> obj_prefix;
				float float_data;
				GLushort glushort_data;

				if (obj_prefix == 'v')
				{
					while (line_sstm_mdl >> float_data)
					{
						pos_vtx.push_back(float_data);
					}
				}
				if (obj_prefix == 'c')
				{
					while (line_sstm_mdl >> float_data)
					{
						clr_vtx.push_back(float_data);
					}
				}
				if (obj_prefix == 'x')
				{
					while (line_sstm_mdl >> float_data)
					{
						tex_coor.push_back(float_data);
					}
				}
				if (obj_prefix == 't')
				{
					while (line_sstm_mdl >> glushort_data)
					{
						gl_tri_primitives.push_back(glushort_data);
					}
					Model.primitive_type = GL_TRIANGLES;
				}
				if (obj_prefix == 'l')
				{
					while (line_sstm_mdl >> glushort_data)
					{
						gl_tri_primitives.push_back(glushort_data);
					}
					Model.primitive_type = GL_LINES;
				}
				if (obj_prefix == 'f')
				{
					while (line_sstm_mdl >> glushort_data)
					{
						gl_tri_primitives.push_back(glushort_data);
					}
					Model.primitive_type = GL_TRIANGLE_FAN;
				}
				if (obj_prefix == 'p')
				{
					while (line_sstm_mdl >> glushort_data)
					{
						gl_tri_primitives.push_back(glushort_data);
					}
					Model.primitive_type = GL_POINTS;
				}
			}
			// Set VAO


			glCreateBuffers(1, &vbo);
			glNamedBufferStorage(vbo, sizeof(glm::vec2) * pos_vtx.size() + sizeof(glm::vec3) * clr_vtx.size() + sizeof(glm::vec2) * tex_coor.size(), nullptr, GL_DYNAMIC_STORAGE_BIT);
			glNamedBufferSubData(vbo, 0, sizeof(glm::vec2) * pos_vtx.size(), pos_vtx.data());
			glNamedBufferSubData(vbo, sizeof(glm::vec2) * pos_vtx.size(), sizeof(glm::vec3) * clr_vtx.size(), clr_vtx.data());
			glNamedBufferSubData(vbo, sizeof(glm::vec2) * pos_vtx.size() + sizeof(glm::vec3) * clr_vtx.size(),
				sizeof(glm::vec2) * tex_coor.size(), tex_coor.data());


			glCreateVertexArrays(1, &vao);

			glEnableVertexArrayAttrib(vao, 0);
			glVertexArrayVertexBuffer(vao, 0, vbo, 0, sizeof(glm::vec2));
			glVertexArrayAttribFormat(vao, 0, 2, GL_FLOAT, GL_FALSE, 0);
			glVertexArrayAttribBinding(vao, 0, 0);

			glEnableVertexArrayAttrib(vao, 1);
			glVertexArrayVertexBuffer(vao, 1, vbo, sizeof(glm::vec2) * pos_vtx.size(), sizeof(glm::vec3));
			glVertexArrayAttribFormat(vao, 1, 3, GL_FLOAT, GL_FALSE, 0);
			glVertexArrayAttribBinding(vao, 1, 1);

			glEnableVertexArrayAttrib(vao, 2);
			glVertexArrayVertexBuffer(vao, 2, vbo, sizeof(glm::vec2) * pos_vtx.size() + sizeof(glm::vec3) * clr_vtx.size(), sizeof(glm::vec2));
			glVertexArrayAttribFormat(vao, 2, 2, GL_FLOAT, GL_FALSE, 0);
			glVertexArrayAttribBinding(vao, 2, 2);

			glCreateBuffers(1, &ebo);
			glNamedBufferStorage(ebo, sizeof(GLushort) * gl_tri_primitives.size(), gl_tri_primitives.data(), GL_DYNAMIC_STORAGE_BIT);
			glVertexArrayElementBuffer(vao, ebo);
			glBindVertexArray(0);

			Model.vaoid = vao;
			Model.primitive_cnt = (unsigned int)gl_tri_primitives.size();
			Model.draw_cnt = (unsigned int)gl_tri_primitives.size();
			models[model_name] = Model;

			//delete vbo, ebo

			glDeleteBuffers(1, &vbo);
			glDeleteBuffers(1, &ebo);
			std::cout << model_name << " model created" << std::endl;
		}
	}
}

/*
* get all he shader programs
*/
void GLApp::init_shdrpgms() {

	insert_shdrpgm("image", "../shaders/image.vert", "../shaders/image.frag");
	std::cout << "test shader program: " << "image-shdrpgm" << std::endl;
	insert_shdrpgm("shape", "../shaders/shape.vert", "../shaders/shape.frag");
	std::cout << "test shader program: " << "shape-shdrpgm" << std::endl;
}

/*
* calculate the matrix of a object
*/
glm::mat3 Getmatrix(glm::vec2 position, glm::vec2 scale, float rotation) {
	glm::mat3 Scale
	{
		scale.x, 0.0f, 0.0f,
			0.0f, scale.y, 0.0f,
			0.0f, 0.0f, 1.0f
	};
	glm::mat3 Rotate
	{
		cos(rotation), sin(rotation), 0.0f,
			-sin(rotation), cos(rotation), 0.0f,
			0.0f, 0.0f, 1.0f
	};
	glm::mat3 Translate
	{
		1.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			position.x, position.y, 1.0f
	};

	glm::mat3 mat = Translate  * Scale * Rotate;
	return mat;
}

/*
* update and draw objects
*/
void GLApp::Update(float time)
{

	//clear screen
	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	//update window bar
	std::stringstream sstr;
	sstr << window->title << " FPS: " << window->fps;
	glfwSetWindowTitle(window->ptr_window, sstr.str().c_str());


	//draw objects
	//int i = 0;
	//while (i < 6) {
	for (long i = 0; i < (long) objectFactory->NumberOfObjects(); i++) {
		GLuint tex_test;
		glm::mat3 mat_test;
		float pos_x;
		float pos_y;
		float orientation;
		float scaling_x;
		float scaling_y;

		//get texture		
		Texture* tex_pt = static_cast<Texture*>(objectFactory->getObjectWithID(i)->GetComponent(ComponentType::Texture));
		tex_test = tex_pt->texturepath;
		
		//get orientation
		Transform* tran_pt = static_cast<Transform*>(objectFactory->getObjectWithID(i)->GetComponent(ComponentType::Transform));
		orientation = tran_pt->Rotation;

		//check debug
		if (input::IsPressed(KEY::p))
		{
			graphics_debug = !graphics_debug;
			std::cout << "graphics_debug" << std::endl;
		}

		//if debug mode get pos and scale from body component
		if (graphics_debug && objectFactory->getObjectWithID(i)->GetComponent(ComponentType::Body) != nullptr) {
			Rectangular* rec_pt = static_cast<Rectangular*>(objectFactory->getObjectWithID(i)->GetComponent(ComponentType::Body));

			if (rec_pt == nullptr)
				break; // Don't continue if there is no body component

			Vec2 botleft = rec_pt->aabb.min;
			Vec2 topright = rec_pt->aabb.max;
			pos_x = (botleft.x + topright.x) / window->width;
			pos_y = (botleft.y + topright.y)  / window->height;
			scaling_x = (topright.x - botleft.x)  / window->width;
			scaling_y = (topright.y - botleft.y)  / window->height;
		}
		else {
			//else get pos and scale from transform component
			pos_x = tran_pt->Position.x * 2.0f / window->width;
			pos_y = tran_pt->Position.y * 2.0f / window->height;
			scaling_x = tran_pt->Scale_x / window->width;
			scaling_y = tran_pt->Scale_y / window->height;
		}
		
		//get matrix
        mat_test =   Getmatrix({pos_x,pos_y}, {scaling_x, scaling_y}, orientation);
		
		if (graphics_debug && objectFactory->getObjectWithID(i)->GetComponent(ComponentType::Body) != nullptr) {
			shdrpgms["shape"].Use();
			// bind VAO of this object's model
			glBindVertexArray(models["square"].vaoid);
			// copy object's model-to-NDC matrix to vertex shader's
			// uniform variable uModelToNDC
			shdrpgms["shape"].SetUniform("uModel_to_NDC", mat_test);
			shdrpgms["shape"].SetUniform("uColor", box_color);
			// call glDrawElements with appropriate arguments
			glDrawElements(models["square"].primitive_type, models["square"].draw_cnt, GL_UNSIGNED_SHORT, 0);

			// unbind VAO and unload shader program
			glBindVertexArray(0);
			shdrpgms["shape"].UnUse();

			// draw movement line for player
			if (static_cast<PlayerControllable*>((objectFactory->getObjectWithID(i))->GetComponent(ComponentType::PlayerControllable)) != nullptr) {
				//get velocity
				Physics* phy_pt = static_cast<Physics*>((objectFactory->getObjectWithID(i))->GetComponent(ComponentType::Physics));
				float Vx = phy_pt->Velocity.x;
				float Vy = phy_pt->Velocity.y;

				//calculate rotation
				orientation = atan2(Vy, Vx);
				
				//get slcae of line based on length of line
				float scale_line_x = sqrt(Vx * Vx + Vy * Vy) / window->width /2;
				float scale_line_y = sqrt(Vx * Vx + Vy * Vy) / window->height /2;
				
				mat_test = Getmatrix({ pos_x,pos_y}, {scale_line_x, scale_line_y}, orientation);
				
				//draw line
				shdrpgms["shape"].Use();
				// bind VAO of this object's model
				glBindVertexArray(models["line"].vaoid);
				// copy object's model-to-NDC matrix to vertex shader's
				// uniform variable uModelToNDC
				shdrpgms["shape"].SetUniform("uModel_to_NDC", mat_test);
				shdrpgms["shape"].SetUniform("uColor", line_color);
				// call glDrawElements with appropriate arguments
				glDrawElements(models["line"].primitive_type, models["line"].draw_cnt, GL_UNSIGNED_SHORT, 0);

				// unbind VAO and unload shader program
				glBindVertexArray(0);
				shdrpgms["shape"].UnUse();
			}

		}
		else {
			// draw object with textuer
			glBindTextureUnit(6, tex_test);
			glBindTexture(GL_TEXTURE_2D, tex_test);
			glTextureSubImage2D(tex_test, 0, 0, 0, window->width, window->height, GL_RGBA, GL_UNSIGNED_BYTE, 0);
			// load shader program in use by this object
			shdrpgms["image"].Use();
			// bind VAO of this object's model
			glBindVertexArray(models["square"].vaoid);
			// copy object's model-to-NDC matrix to vertex shader's
			// uniform variable uModelToNDC
			shdrpgms["image"].SetUniform("uModel_to_NDC", mat_test);

			// tell fragment shader sampler uTex2d will use texture image unit 6
			GLuint tex_loc = glGetUniformLocation(shdrpgms["image"].GetHandle(), "uTex2d");
			glUniform1i(tex_loc, 6);

			// call glDrawElements with appropriate arguments
			glDrawElements(models["square"].primitive_type, models["square"].draw_cnt, GL_UNSIGNED_SHORT, 0);

			// unbind VAO and unload shader program
			glBindVertexArray(0);
			shdrpgms["image"].UnUse();
		}
    }

	
	glfwSwapBuffers(window->ptr_window);
}

/*
* clean up
*/
void GLApp::cleanup()
{
	//delete all models
	for (auto& model : models)
	{
		glDeleteVertexArrays(1, &model.second.vaoid);
	}
	//delete all shader programs
	for (auto& shdrpgm : shdrpgms)
	{
		glDeleteProgram(shdrpgm.second.GetHandle());
	}
}

/*
* read png file
*/
GLuint GLApp::setup_texobj(const char* pathname)
{
	int width{ }, height{  }, image_channels;

	// load image data from file into client memory
	unsigned char* ptr_texels = stbi_load(pathname, &width, &height, &image_channels, STBI_rgb_alpha);
	if (!ptr_texels)
	{
		std::cout << "Unable to load texture file: " << pathname << "\n";
		std::exit(EXIT_FAILURE);
	}
	else {
		std::cout << "Loaded texture file: " << pathname << "\n";
		std::cout << "Image width: " << width << "\n";
		std::cout << "Image height: " << height << "\n";
	}

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

/*
*  load shader program and store it in map
*/
void GLApp::insert_shdrpgm(std::string shdr_pgm_name, std::string vtx_shdr, std::string frg_shdr)
{
	std::vector<std::pair<GLenum, std::string>> shdr_files
	{
		std::make_pair(GL_VERTEX_SHADER, vtx_shdr),
		std::make_pair(GL_FRAGMENT_SHADER, frg_shdr)
	};

	GLSLShader shdr_pgm;
	shdr_pgm.CompileLinkValidate(shdr_files);
	if (GL_FALSE == shdr_pgm.IsLinked())
	{
		std::cout << "Unable to compile/link/validate shader programs\n";
		std::cout << shdr_pgm.GetLog() << "\n";
		std::exit(EXIT_FAILURE);
	}

	// add compiled, linked, and validated shader program to
	// std::map container GLApp::shdrpgms
	GLApp::shdrpgms[shdr_pgm_name] = shdr_pgm;
}

