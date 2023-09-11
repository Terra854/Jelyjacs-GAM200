
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
std::map<std::string , GLApp::GLObject> GLApp::objects;
std::map<std::string , GLuint> GLApp::textures;

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
	
	init_scene();
}

void GLApp::init_scene()
{
	//read file
	std::ifstream ifs("../scenes/game.scn", std::ios::in);
	if (!ifs)
	{
		std::cout <<
			"Error : Unable to open scene file : " << std::endl;
		exit(EXIT_FAILURE);
	}
	ifs.seekg(0, std::ios::beg);
	std::string line;
	getline(ifs, line);
	std::istringstream line_sstm{ line };
	int obj_cnt;
	line_sstm >> obj_cnt;
	while (obj_cnt--)
	{
		GLObject Object;

		// line 1   model name
		getline(ifs, line);
		std::istringstream line_model_name{ line };
		std::string model_name;
		line_model_name >> model_name;
		
		// if model with name model_name is not present in std::map container
		//called models, then add this model to the container
		if (models.find(model_name) != models.end())
		{
			Object.mdl_ref = models.find(model_name);
		}
		else
		{
			GLModel Model;

			// Read from meshes files
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


			std::vector < float > pos_vtx;
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
				if (obj_prefix == 't')
				{
					while (line_sstm_mdl >> glushort_data)
					{
						gl_tri_primitives.push_back(glushort_data);
					}
					Model.primitive_type = GL_TRIANGLES;
					}
					if (obj_prefix == 'f')
					{
						while (line_sstm_mdl >> glushort_data)
						{
							gl_tri_primitives.push_back(glushort_data);
						}
						Model.primitive_type = GL_TRIANGLE_FAN;
					}

				}
				// Set VAO
				glCreateBuffers(1, &vbo);
				glNamedBufferStorage(vbo, sizeof(float) * pos_vtx.size(), pos_vtx.data(), GL_DYNAMIC_STORAGE_BIT);

				glCreateVertexArrays(1, &vao);
				glEnableVertexArrayAttrib(vao, 0);
				glVertexArrayVertexBuffer(vao, 0, vbo, 0, 2 * sizeof(float));
				glVertexArrayAttribFormat(vao, 0, 2, GL_FLOAT, GL_FALSE, 0);
				glVertexArrayAttribBinding(vao, 0, 0);

				glCreateBuffers(1, &ebo);
				glNamedBufferStorage(ebo, sizeof(GLushort) * gl_tri_primitives.size(), gl_tri_primitives.data(), GL_DYNAMIC_STORAGE_BIT);
				glVertexArrayElementBuffer(vao, ebo);
				glBindVertexArray(0);

				Model.vaoid = vao;
				Model.primitive_cnt = gl_tri_primitives.size();
				Model.draw_cnt = gl_tri_primitives.size();

				models[model_name] = Model;
				Object.mdl_ref = models.find(model_name);
			}

		std::cout << "model name: " << model_name << std::endl;

		// line2  object number
		getline(ifs, line);
		std::istringstream line_model_obj_num{ line };
		std::string model_object;
		line_model_obj_num >> model_object;
		std::cout << "object: " << model_object << std::endl;

		// line3 object texture
		getline(ifs, line);
		std::istringstream line_model_texture{ line };
		std::string model_texture;
		line_model_texture >> model_texture;
	
		if (textures.find(model_texture) != textures.end()) {
			Object.tex_ref = textures.find(model_texture);
		}
		else {
			GLuint Texture;
			std::string texture_file = "../Assest/Picture/" + model_texture + ".png";
			Texture = setup_texobj(texture_file.c_str());
			textures[model_texture] = Texture;
			Object.tex_ref = textures.find(model_texture);
		}
		std::cout << "texture: " << model_texture << std::endl;

		//line 4  object shader program
		getline(ifs, line);
		std::istringstream line_model_obj_shader{ line };
		std::string object_shader_program, object_shader_vertex, object_shader_fragment;
		line_model_obj_shader >> object_shader_program;
		line_model_obj_shader >> object_shader_vertex;
		line_model_obj_shader >> object_shader_fragment;
		//if shader program listed in the scene file is not present in
		//std::map container called shdrpgms, then add this model to the container
		if (shdrpgms.find(object_shader_program) != shdrpgms.end())
		{
			Object.shd_ref = shdrpgms.find(object_shader_program);
		}
		else
		{
			insert_shdrpgm(object_shader_program, object_shader_vertex, object_shader_fragment);
			Object.shd_ref = shdrpgms.find(object_shader_program);
		}
		objects[model_object] = Object;
		std::cout << "shader program: " << object_shader_program << std::endl;

		//line 5 position
		getline(ifs, line);
		std::istringstream line_model_obj_pos{ line };
		line_model_obj_pos >> Object.position.x >> Object.position.y;
		std::cout << "position: " << Object.position.x << " " << Object.position.y << std::endl;

		//line 6 scale
		getline(ifs, line);
		std::istringstream line_model_obj_scale{ line };
		line_model_obj_scale >> Object.scaling.x >> Object.scaling.y;
		std::cout << "scale: " << Object.scaling.x << " " << Object.scaling.y << std::endl;

		//line 7 orientation
		getline(ifs, line);
		std::istringstream line_model_obj_orientation{ line };
		line_model_obj_orientation >> Object.orientation;
		Object.orientation *= 3.1425f / 180.0f;
		std::cout << "orientation: " << Object.orientation << std::endl;

	}
}

void GLApp::update ( )
{
	for (std::map <std::string, GLObject> ::iterator obj = objects.begin(); obj != objects.end(); ++obj)
	{
		obj->second.update();
	}
}

void GLApp::GLObject::update()
{
	glm::mat3 Scale
	{
		scaling.x, 0, 0,
			0, scaling.y, 0,
			0, 0, 1
	};

	

	glm::mat3 Rotate
	{
		cos(orientation), sin(orientation), 0,
			-sin(orientation), cos(orientation), 0,
			0, 0, 1
	};

	glm::mat3 Translate
	{
		1, 0, 0,
			0, 1, 0,
			position.x, position.y, 1
	};

	mdl_to_ndc_xform =  Translate * Rotate * Scale;

}
void GLApp::draw ()
{
	std::stringstream ss;
	ss << std::fixed;

	ss << std::fixed;
	ss.precision(2);

	glfwSetWindowTitle(GLHelper::ptr_window, ss.str().c_str());

	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
	// clear back buffer as before
	glClear(GL_COLOR_BUFFER_BIT);

	// draw all objects
	for (std::map <std::string, GLObject> ::iterator obj = objects.begin(); obj != objects.end(); ++obj)
	{
		obj->second.draw();
	}

}


void GLApp::cleanup ()
{
}


GLuint GLApp::setup_texobj(const char* pathname)
{
	int width{ }, height{  }, image_channels;
	
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



void GLApp::GLObject::draw() const
{
	glBindTextureUnit(6, tex_ref->second);
	static int i = 1;
	if (i == 1) {
		i++;
		std::cout << "draw " << tex_ref->first << std::endl;
	}
	// load shader program in use by this object
	shd_ref->second.Use();
	// bind VAO of this object's model
	glBindVertexArray(mdl_ref->second.vaoid);
	// copy object's model-to-NDC matrix to vertex shader's
	// uniform variable uModelToNDC
	shd_ref->second.SetUniform("uModel_to_NDC", mdl_to_ndc_xform);

	// tell fragment shader sampler uTex2d will use texture image unit 6
	GLuint tex_loc = glGetUniformLocation(mdl_ref->second.shdr_pgm.GetHandle(), "uTex2d");
	glUniform1i(tex_loc, 6);
	// call glDrawElements with appropriate arguments
	glDrawElements(mdl_ref->second.primitive_type, mdl_ref->second.draw_cnt, GL_UNSIGNED_SHORT, NULL);

	// unbind VAO and unload shader program
	glBindVertexArray(0);
	shd_ref->second.UnUse();
}