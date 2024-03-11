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
#include <components/Animation.h>
#include <LevelEditor.h>
#include "../Assets Manager/asset_manager.h"
#include "components/Particle.h"
#include <Gizmo.h>
#include <SceneManager.h>
#include <Font.h>
#include <components/Text.h>

/* Objects with file scope
----------------------------------------------------------------------------- */


//debug 
bool graphics_debug{ false };//press l to activate debug mode
glm::vec3 box_color{ 0.0f, 1.0f, 1.0f };
glm::vec3 red_box_color{ 1.0f, 0.0f, 0.0f };
glm::vec3 green_box_color{ 0.0f, 1.0f, 0.0f };
glm::vec3 white_box_color{ 1.0f, 1.0f, 1.0f };
glm::vec3 line_color{ 0.0f, 1.0f, 0.0f };




//Global pointer to GLApp
GLApp* app = NULL;
GLApp::GLApp()
{
	app = this;
}
//ParticleSystem particleSystem;

/*  _________________________________________________________________________ */
/*
* Initialize() is called once, at program start.
*/
void GLApp::Initialize()
{
	glClearColor(1.f, 1.f, 1.f, 1.f);

	glViewport(0, 0, window->width_init, window->height_init);
	
	// enable alpha blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	
}



/*
* get all he models from the list.txt file and store them in the models map
* @Guo Chen, this is the modified version of your init_model that works with assetmanager, you might want to delete your other one and this line
*/
void GLApp::insert_models(std::string model_name) {

		//check if model already exists
		if (AssetManager::modelexist(model_name)) {
			GLModel Model;
			std::ifstream ifs_msh{ "Asset/meshes/" + model_name + ".msh", std::ios::in };
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
				if (obj_prefix == 'o')
				{
					while (line_sstm_mdl >> glushort_data)
					{
						gl_tri_primitives.push_back(glushort_data);
					}
					Model.primitive_type = GL_LINE_LOOP;
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

			AssetManager::addmodel(model_name, Model);

			//delete vbo, ebo

			glDeleteBuffers(1, &vbo);
			glDeleteBuffers(1, &ebo);
			std::cout << model_name << " model created" << std::endl;
		}
}



/*  _________________________________________________________________________ */
/*
* update and draw objects
*/
void GLApp::Update()
{
	
		//check debug
		if (input::IsPressed(KEY::l))
		{
			graphics_debug = !graphics_debug;
			std::cout << "graphics_debug" << std::endl;
		}

		//clear screen
		glClearColor(0.11f, 0.094f, 0.161f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//update window bar
		std::stringstream sstr;
		sstr << "Feline Felony";
		glfwSetWindowTitle(window->ptr_window, sstr.str().c_str());

		//draw objects

		for (auto& l : SceneManager::layers) {
			if (l.second.first.isVisible) {
				for (auto& object : l.second.second) {

					if (!object)
						continue; // Skip to the next object if the current one is a nullptr

					GLuint tex_test;
					Animation* ani_pt = nullptr;
					Mat3 mat_test;
					Vec2 pos;
					float orientation;
					Vec2 scaling;
					Vec2 window_scaling;
					//bool texture_bool = true;
					//get texture		
					Texture* tex_pt = static_cast<Texture*>(object->GetComponent(ComponentType::Texture));

					//get text
					Text* text = static_cast<Text*>(object->GetComponent(ComponentType::Text));

					

					//get animation
					ani_pt = static_cast<Animation*>(object->GetComponent(ComponentType::Animation));

					if (!tex_pt && !ani_pt && !text)
						continue; // There is nothing to render, so skip to the next object

					if (tex_pt)
						tex_test = AssetManager::textureval(tex_pt->textureName);
					else if (ani_pt)
						tex_test = ani_pt->animation_tex_obj;

					//get orientation
					Transform* tran_pt = static_cast<Transform*>(object->GetComponent(ComponentType::Transform));

					// skip to next object if there is no transformation
					if (!tran_pt)
						continue;
					else
						orientation = tran_pt->Rotation;

					// get pos and scale from transform component
					pos.x = tran_pt->Position.x * 2.0f / window->width_init;
					pos.y = tran_pt->Position.y * 2.0f / window->height_init;
					scaling.x = tran_pt->Scale.x / window->width_init;
					scaling.y = tran_pt->Scale.y / window->height_init;


					//get matrix
					mat_test = Mat3Translate(pos) * Mat3Scale(scaling) * Mat3RotDeg(orientation);

					window_scaling = { (float)window->width / (float)window->width_init, (float)window->height / (float)window->height_init };

					mat_test = Mat3Scale(window_scaling.x, window_scaling.y) * mat_test;
					// matrix after camrea

					if (!l.second.first.static_layer)
						mat_test = camera2D->world_to_ndc * mat_test;


					// draw image with texture
					if (tex_pt) {
						// draw object with textuer
						glBindTextureUnit(6, tex_test);
						glBindTexture(GL_TEXTURE_2D, tex_test);
						// load shader program in use by this object
						AssetManager::shaderval("image").Use();
						// bind VAO of this object's model
						glBindVertexArray(AssetManager::modelval("square").vaoid);
						// copy object's model-to-NDC matrix to vertex shader's
						// uniform variable uModelToNDC
						AssetManager::shaderval("image").SetUniform("uModel_to_NDC", mat_test.ToGlmMat3());
						AssetManager::shaderval("image").SetUniform("uOpacity", tex_pt->opacity);

						// tell fragment shader sampler uTex2d will use texture image unit 6
						GLuint tex_loc = glGetUniformLocation(AssetManager::shaderval("image").GetHandle(), "uTex2d");
						glUniform1i(tex_loc, 6);

						// call glDrawElements with appropriate arguments
						glDrawElements(AssetManager::modelval("square").primitive_type, AssetManager::modelval("square").draw_cnt, GL_UNSIGNED_SHORT, 0);

						// unbind VAO and unload shader program
						glBindVertexArray(0);
						AssetManager::shaderval("image").UnUse();
					}
					else if (ani_pt) {
						// if is player
						if (static_cast<PlayerControllable*>(object->GetComponent(ComponentType::PlayerControllable)) != nullptr) {
							ParticleSystem* particleSystem = static_cast<ParticleSystem*>(object->GetComponent(ComponentType::ParticleSystem));
							//if w is pressed
							
							if (particleSystem != nullptr)

							{
								if (input::IsPressed(KEY::w)) {
									particleSystem->prticle_state = ParticleState::Prticle_Start;
									
								}
								particleSystem->Update(object);
								
							}
							// draw object with animation

							ani_pt->Update_player();
						}
						else {
							// frame number change to 0 if animation type change

							ani_pt->Update_objects();

						}
						//ani_pt->opacity = 0.5f;
						// render animation
						glBindTextureUnit(6, tex_test);
						glBindTexture(GL_TEXTURE_2D, tex_test);
						//glTextureSubImage2D(tex_test, 0, 0, 0, window->width, window->height, GL_RGBA, GL_UNSIGNED_BYTE, 0);
						// load shader program in use by this object
						AssetManager::shaderval("image").Use();
						// bind VAO of this object's model
						glBindVertexArray(ani_pt->animation_Map[ani_pt->current_type][ani_pt->frame_num].vaoid);
						// copy object's model-to-NDC matrix to vertex shader's
						// uniform variable uModelToNDC
						AssetManager::shaderval("image").SetUniform("uModel_to_NDC", mat_test.ToGlmMat3());
						AssetManager::shaderval("image").SetUniform("uOpacity", ani_pt->opacity);
						// tell fragment shader sampler uTex2d will use texture image unit 6
						GLuint tex_loc = glGetUniformLocation(AssetManager::shaderval("image").GetHandle(), "uTex2d");
						glUniform1i(tex_loc, 6);

						// call glDrawElements with appropriate arguments
						glDrawElements(ani_pt->animation_Map[ani_pt->current_type][ani_pt->frame_num].primitive_type, ani_pt->animation_Map[ani_pt->current_type][ani_pt->frame_num].draw_cnt, GL_UNSIGNED_SHORT, 0);

						// unbind VAO and unload shader program
						glBindVertexArray(0);
						AssetManager::shaderval("image").UnUse();

						ani_pt->Update_time();

					}

					if (text != nullptr) {
						// draw text
						SetFont(FONT::GeoRegular);
						float text_width{ static_cast<float>(find_width(text->text,FONT::GeoRegular))};
						DrawText(text->text, pos.x * window->width / 2.f - text_width/2 , pos.y * window->height / 2.f, text->fontSize);
					}
#if defined(DEBUG) | defined(_DEBUG)
					if (graphics_debug && object->GetComponent(ComponentType::Body) != nullptr) {

						Rectangular* rec_pt = static_cast<Rectangular*>(object->GetComponent(ComponentType::Body));

						if (rec_pt == nullptr)
							break; // Don't continue if there is no body component

						Vec2 botleft = rec_pt->aabb.min;
						Vec2 topright = rec_pt->aabb.max;

						if (rec_pt->active) {
							drawline(Vec2(topright.x, botleft.y), botleft, box_color);
							drawline(topright, Vec2(topright.x, botleft.y), box_color);
							drawline(topright, Vec2(botleft.x, topright.y), box_color);
							drawline(Vec2(botleft.x, topright.y), botleft, box_color);
						}
						else {
							drawline(Vec2(topright.x, botleft.y), botleft, red_box_color);
							drawline(topright, Vec2(topright.x, botleft.y), red_box_color);
							drawline(topright, Vec2(botleft.x, topright.y), red_box_color);
							drawline(Vec2(botleft.x, topright.y), botleft, red_box_color);
						}
						// draw movement line for player
						if (static_cast<PlayerControllable*>(object->GetComponent(ComponentType::PlayerControllable)) != nullptr) {
							//get velocity
							Physics* phy_pt = static_cast<Physics*>(object->GetComponent(ComponentType::Physics));

							// Make sure it's not null pointer before continuing
							if (phy_pt != nullptr) {

								float Vx = phy_pt->Velocity.x;
								float Vy = phy_pt->Velocity.y;

								//calculate rotation
								orientation = atan2(Vy, Vx);

								//get slcae of line based on length of line
								Vec2 scale_line = { sqrt(Vx * Vx + Vy * Vy) / window->width_init / 2, sqrt(Vx * Vx + Vy * Vy) / window->height_init / 2 };

								mat_test = Mat3Translate(pos) * Mat3Scale(scale_line) * Mat3RotRad(orientation);
								mat_test = Mat3Scale(window_scaling.x, window_scaling.y) * mat_test;
								mat_test = camera2D->world_to_ndc * mat_test;
								//draw line
								AssetManager::shaderval("shape").Use();
								// bind VAO of this object's model
								glBindVertexArray(AssetManager::modelval("line").vaoid);
								// copy object's model-to-NDC matrix to vertex shader's
								// uniform variable uModelToNDC
								AssetManager::shaderval("shape").SetUniform("uModel_to_NDC", mat_test.ToGlmMat3());
								AssetManager::shaderval("shape").SetUniform("uColor", line_color);
								// call glDrawElements with appropriate arguments
								glDrawElements(AssetManager::modelval("line").primitive_type, AssetManager::modelval("line").draw_cnt, GL_UNSIGNED_SHORT, 0);

								// unbind VAO and unload shader program
								glBindVertexArray(0);
								AssetManager::shaderval("shape").UnUse();
							}
						}

					}
#endif
				}
			}


		}
		for (auto& l : SceneManager::layers) {
			if (l.second.first.isVisible) {
				for (auto& object : l.second.second) {
					GLuint tex_test;
					Animation* ani_pt = nullptr;
					Mat3 mat_test;
					Vec2 pos;
					float orientation;
					Vec2 scaling;
					Vec2 window_scaling;
					//bool texture_bool = true;
					//get texture		
					Texture* tex_pt = static_cast<Texture*>(object->GetComponent(ComponentType::Texture));

					//get text
					Text* text = static_cast<Text*>(object->GetComponent(ComponentType::Text));



					//get animation
					ani_pt = static_cast<Animation*>(object->GetComponent(ComponentType::Animation));

					if (!tex_pt && !ani_pt && !text)
						continue; // There is nothing to render, so skip to the next object

					if (tex_pt)
						tex_test = AssetManager::textureval(tex_pt->textureName);
					else if (ani_pt)
						tex_test = ani_pt->animation_tex_obj;

					//get orientation
					Transform* tran_pt = static_cast<Transform*>(object->GetComponent(ComponentType::Transform));

					// skip to next object if there is no transformation
					if (!tran_pt)
						continue;
					else
						orientation = tran_pt->Rotation;

					// get pos and scale from transform component
					pos.x = tran_pt->Position.x * 2.0f / window->width_init;
					pos.y = tran_pt->Position.y * 2.0f / window->height_init;
					scaling.x = tran_pt->Scale.x / window->width_init;
					scaling.y = tran_pt->Scale.y / window->height_init;


					//get matrix
					mat_test = Mat3Translate(pos) * Mat3Scale(scaling) * Mat3RotDeg(orientation);

					window_scaling = { (float)window->width / (float)window->width_init, (float)window->height / (float)window->height_init };

					mat_test = Mat3Scale(window_scaling.x, window_scaling.y) * mat_test;
					// matrix after camrea

					if (!l.second.first.static_layer)
						mat_test = camera2D->world_to_ndc * mat_test;


					// draw image with texture
					if (ani_pt) {
						// if is player
						if (static_cast<PlayerControllable*>(object->GetComponent(ComponentType::PlayerControllable)) != nullptr) {
							ParticleSystem* particleSystem = static_cast<ParticleSystem*>(object->GetComponent(ComponentType::ParticleSystem));
							//if w is pressed

							if (particleSystem != nullptr)

							{
								if (input::IsPressed(KEY::w)) {
									particleSystem->prticle_state = ParticleState::Prticle_Start;

								}
								particleSystem->Update(object);

							}
						}
					}
				}
			}




		}



#if defined(DEBUG) | defined(_DEBUG)
		// Draw the bove around the selected object
		if (level_editor->selected && level_editor->selectedNum >= 0) {
			//Animation* a = static_cast<Animation*>(objectFactory->getObjectWithID(level_editor->selectedNum)->GetComponent(ComponentType::Animation));
			//Texture* te = static_cast<Texture*>(objectFactory->getObjectWithID(level_editor->selectedNum)->GetComponent(ComponentType::Texture));
			Transform* tr = static_cast<Transform*>(objectFactory->getObjectWithID(level_editor->selectedNum)->GetComponent(ComponentType::Transform));

			//GLint width, height;
			Vec2 botleft, topright;

			botleft = tr->Position + -tr->Scale / 2.f;
			topright = tr->Position + tr->Scale / 2.f;

			drawline(Vec2(topright.x, botleft.y), botleft, white_box_color);
			drawline(topright, Vec2(topright.x, botleft.y), white_box_color);
			drawline(topright, Vec2(botleft.x, topright.y), white_box_color);
			drawline(Vec2(botleft.x, topright.y), botleft, white_box_color);
			/*
			if (te != nullptr) {
				// Bind the texture
				glBindTexture(GL_TEXTURE_2D, AssetManager::textureval(te->textureName));

				// Get the texture width
				glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);

				// Get the texture height
				glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);

				// Unbind the texture
				glBindTexture(GL_TEXTURE_2D, 0);

				//botleft = tr->Position + Vec2(-(width / 2.f), -(height / 2.f));
				botleft = tr->Position + -tr->Scale / 2.f;
				//topright = tr->Position + Vec2(width / 2.f, height / 2.f);
				topright = tr->Position + tr->Scale / 2.f;

				drawline(Vec2(topright.x, botleft.y), botleft, white_box_color);
				drawline(topright, Vec2(topright.x, botleft.y), white_box_color);
				drawline(topright, Vec2(botleft.x, topright.y), white_box_color);
				drawline(Vec2(botleft.x, topright.y), botleft, white_box_color);
			}
			else if (a != nullptr) {
				// TODO: Get the size of the animations
				// Right now, it's hardcoded to 64x64
				botleft = tr->Position + Vec2(-32.f, -32.f);
				topright = tr->Position + Vec2(32.f, 32.f);

				drawline(Vec2(topright.x, botleft.y), botleft, white_box_color);
				drawline(topright, Vec2(topright.x, botleft.y), white_box_color);
				drawline(topright, Vec2(botleft.x, topright.y), white_box_color);
				drawline(Vec2(botleft.x, topright.y), botleft, white_box_color);
			}*/

			gizmo.SetObject(tr);

			if (engine->isPaused())
				gizmo.RenderGizmo();
		}
#endif
	
	
}

/*  _________________________________________________________________________ */
/*
* clean up
*/
void GLApp::cleanup()
{
	
}

/*  _________________________________________________________________________ */
/*
* read png file
* @param pathname The path of the png file
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
#if defined(DEBUG) | defined(_DEBUG)
		std::cout << "Loaded texture file: " << pathname << "\n";
		std::cout << "Image width: " << width << "\n";
		std::cout << "Image height: " << height << "\n";
#endif
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

/*  _________________________________________________________________________ */
/*
*  load shader program and store it in map
* @param shdr_pgm_name The name of the shader program
* @param vtx_shdr The vertex shader file
* @param frg_shdr The fragment shader file
*/
void GLApp::insert_shdrpgm(std::string shdr_pgm_name, std::string vtx_shdr, std::string frg_shdr)
{
	std::vector<std::pair<GLenum, std::string>> shdr_files
	{
		std::make_pair(GL_VERTEX_SHADER, vtx_shdr),
		std::make_pair(GL_FRAGMENT_SHADER, frg_shdr)
	};

	std::cout << shdr_pgm_name << " | " << vtx_shdr << " | " << frg_shdr << std::endl;

	GLSLShader shdr_pgm;
	shdr_pgm.CompileLinkValidate(shdr_files);
	if (GL_FALSE == shdr_pgm.IsLinked())
	{
		std::cout << "Unable to compile/link/validate shader programs\n";
		std::cout << shdr_pgm.GetLog() << "\n";
		std::exit(EXIT_FAILURE);
	}

	// add compiled, linked, and validated shader program to
	

	// @Chen Guo when you removed your own map, leave this here ^delete the line above
	AssetManager::addshader(shdr_pgm_name, shdr_pgm);
}

/*  _________________________________________________________________________ */
/*
* a function to draw line
* @param start The start point of the line
* @param end The end point of the line
* @param color The color of the line
* 
*/
void GLApp::drawline(Vec2 start, Vec2 end, glm::vec3 color) {
	float pos_x;
	float pos_y;
	float orientation;
	float scaling_x;
	float scaling_y;
	orientation = atan2(end.y - start.y, end.x - start.x);

	scaling_x = abs(end.x - start.x) * 2 / window->width;
	scaling_y = abs(end.y - start.y) * 2 / window->height;
	pos_x = start.x * 2.0f / window->width;
	pos_y = start.y * 2.0f / window->height;

	Mat3 mat_test;
	mat_test = Mat3Translate(pos_x, pos_y) * Mat3Scale(scaling_x, scaling_y) * Mat3RotRad(orientation);
	Vec2 window_sacling = {(float) window->width / window->width, (float)window->height / window->height };
	mat_test = Mat3Scale(window_sacling.x, window_sacling.y) * mat_test;
	mat_test = camera2D->world_to_ndc * mat_test;
	//draw line
	AssetManager::shaderval("shape").Use();
	// bind VAO of this object's model
	glBindVertexArray(AssetManager::modelval("line").vaoid);
	// copy object's model-to-NDC matrix to vertex shader's
	// uniform variable uModelToNDC
	AssetManager::shaderval("shape").SetUniform("uModel_to_NDC", mat_test.ToGlmMat3());
	AssetManager::shaderval("shape").SetUniform("uColor", color);
	// call glDrawElements with appropriate arguments
	glDrawElements(AssetManager::modelval("line").primitive_type, AssetManager::modelval("line").draw_cnt, GL_UNSIGNED_SHORT, 0);

	// unbind VAO and unload shader program
	glBindVertexArray(0);
	AssetManager::shaderval("shape").UnUse();
}

/*  _________________________________________________________________________ */
/*
* a function to draw triangle
* @param tri_pos The position of the triangle
* @param tri_scale The scale of the triangle
* @param tri_r The rotation of the triangle
* @param tri_color The color of the triangle
* */

void GLApp::drawtriangle(Vec2 tri_pos, Vec2 tri_scale, float tri_r, glm::vec3 tri_color) {
	float pos_x;
	float pos_y;
	float scaling_x;
	float scaling_y;

	scaling_x = tri_scale.x * 2.0f / window->width_init;
	scaling_y = tri_scale.y * 2.0f / window->height_init;
	pos_x = tri_pos.x * 2.0f / window->width_init;
	pos_y = tri_pos.y * 2.0f / window->height_init;

	Mat3 mat_test;
	mat_test = Mat3Translate(pos_x, pos_y) * Mat3Scale(scaling_x, scaling_y) * Mat3RotDeg(tri_r);
	Vec2 window_sacling = { (float)window->width / window->width_init, (float)window->height / window->height_init };
	mat_test = Mat3Scale(window_sacling.x, window_sacling.y) * mat_test;
	mat_test = camera2D->world_to_ndc * mat_test;

	//draw triangle
	AssetManager::shaderval("shape").Use();
	
	// bind VAO of this object's model
	glBindVertexArray(AssetManager::modelval("triangle").vaoid);
	// copy object's model-to-NDC matrix to vertex shader's
	// uniform variable uModelToNDC
	AssetManager::shaderval("shape").SetUniform("uModel_to_NDC", mat_test.ToGlmMat3());
	AssetManager::shaderval("shape").SetUniform("uColor", tri_color);
	// call glDrawElements with appropriate arguments
	glDrawElements(AssetManager::modelval("triangle").primitive_type, AssetManager::modelval("triangle").draw_cnt, GL_UNSIGNED_SHORT, 0);

	// unbind VAO and unload shader program
	glBindVertexArray(0);
	AssetManager::shaderval("shape").UnUse();
}

/*  _________________________________________________________________________ */
/*
* a function to draw circle
* @param c_pos The position of the circle
* @param c_scale The scale of the circle
* @param c_r The rotation of the circle
* @param c_color The color of the circle
* */

void GLApp::drawline_circle(Vec2 l_c_pos, Vec2 l_c_scale, float l_c_width, glm::vec3 l_c_color)
{
	float pos_x;
	float pos_y;
	float scaling_x;
	float scaling_y;

	scaling_x = l_c_scale.x * 2.0f / window->width_init;
	scaling_y = l_c_scale.y * 2.0f / window->height_init;
	pos_x = l_c_pos.x * 2.0f / window->width_init;
	pos_y = l_c_pos.y * 2.0f / window->height_init;

	Mat3 mat_test;
	mat_test = Mat3Translate(pos_x, pos_y) * Mat3Scale(scaling_x, scaling_y);
	Vec2 window_sacling = { (float)window->width / window->width_init, (float)window->height / window->height_init };
	mat_test = Mat3Scale(window_sacling.x, window_sacling.y) * mat_test;
	mat_test = camera2D->world_to_ndc * mat_test;

	glLineWidth(l_c_width);
	//draw line_circle
	AssetManager::shaderval("shape").Use();
	// bind VAO of this object's model
	glBindVertexArray(AssetManager::modelval("line_circle").vaoid);
	// copy object's model-to-NDC matrix to vertex shader's
	// uniform variable uModelToNDC
	AssetManager::shaderval("shape").SetUniform("uModel_to_NDC", mat_test.ToGlmMat3());
	AssetManager::shaderval("shape").SetUniform("uColor", l_c_color);
	// call glDrawElements with appropriate arguments
	glDrawElements(AssetManager::modelval("line_circle").primitive_type, AssetManager::modelval("line_circle").draw_cnt, GL_UNSIGNED_SHORT, 0);

	// unbind VAO and unload shader program
	glBindVertexArray(0);
	AssetManager::shaderval("shape").UnUse();
}

/*  _________________________________________________________________________ */
/*
* a function to draw rect
* @param rec_pos The position of the rect
* @param rec_scale The scale of the rect
* @param rec_r The rotation of the rect
* @param rec_color The color of the rect
* */

void GLApp::draw_rect(Vec2 rec_pos, Vec2 rec_scale, float rec_r, glm::vec3 rec_color)
{
	float pos_x;
	float pos_y;
	float scaling_x;
	float scaling_y;

	scaling_x = rec_scale.x * 2.0f / window->width_init;
	scaling_y = rec_scale.y * 2.0f / window->height_init;
	pos_x = rec_pos.x * 2.0f / window->width_init;
	pos_y = rec_pos.y * 2.0f / window->height_init;

	Mat3 mat_test;
	mat_test = Mat3Translate(pos_x, pos_y) * Mat3Scale(scaling_x, scaling_y) * Mat3RotDeg(rec_r);
	Vec2 window_sacling = { (float)window->width / window->width_init, (float)window->height / window->height_init };
	mat_test = Mat3Scale(window_sacling.x, window_sacling.y) * mat_test;
	mat_test = camera2D->world_to_ndc * mat_test;

	//draw square
	AssetManager::shaderval("shape").Use();
	// bind VAO of this object's model
	glBindVertexArray(AssetManager::modelval("square").vaoid);
	// copy object's model-to-NDC matrix to vertex shader's
	// uniform variable uModelToNDC
	AssetManager::shaderval("shape").SetUniform("uModel_to_NDC", mat_test.ToGlmMat3());
	AssetManager::shaderval("shape").SetUniform("uColor", rec_color);
	// call glDrawElements with appropriate arguments
	glDrawElements(AssetManager::modelval("square").primitive_type, AssetManager::modelval("square").draw_cnt, GL_UNSIGNED_SHORT, 0);

	// unbind VAO and unload shader program
	glBindVertexArray(0);
	AssetManager::shaderval("shape").UnUse();
}


/*  _________________________________________________________________________ */
/*
* a function to draw texture
* @param tex_t The position of the texture
* @param tex_s The scale of the texture
* @param tex_r The rotation of the texture
* @param tex_in The texture
* @param tex_camera The texture is camera
* */

void GLApp::draw_texture(Vec2 tex_t, Vec2 tex_s, float tex_r, GLuint tex_in, bool tex_camera)
{
	float pos_x;
	float pos_y;
	float scaling_x;
	float scaling_y;

	scaling_x = tex_s.x * 2.0f / window->width_init;
	scaling_y = tex_s.y * 2.0f / window->height_init;
	pos_x = tex_t.x * 2.0f / window->width_init;
	pos_y = tex_t.y * 2.0f / window->height_init;

	Mat3 mat_test;
	mat_test = Mat3Translate(pos_x, pos_y) * Mat3Scale(scaling_x, scaling_y) * Mat3RotDeg(tex_r);
	Vec2 window_sacling = { (float)window->width / window->width_init, (float)window->height / window->height_init };
	mat_test = Mat3Scale(window_sacling.x, window_sacling.y) * mat_test;
	if (tex_camera)
		mat_test = camera2D->world_to_ndc * mat_test;
	

	glBindTextureUnit(6, tex_in);
	glBindTexture(GL_TEXTURE_2D, tex_in);
	//draw texture
	AssetManager::shaderval("image").Use();
	// bind VAO of this object's model
	glBindVertexArray(AssetManager::modelval("square").vaoid);
	// copy object's model-to-NDC matrix to vertex shader's
	// uniform variable uModelToNDC
	AssetManager::shaderval("image").SetUniform("uModel_to_NDC", mat_test.ToGlmMat3());
	// tell fragment shader sampler uTex2d will use texture image unit 6
	GLuint tex_loc = glGetUniformLocation(AssetManager::shaderval("image").GetHandle(), "uTex2d");
	glUniform1i(tex_loc, 6);
	// call glDrawElements with appropriate arguments
	glDrawElements(AssetManager::modelval("square").primitive_type, AssetManager::modelval("square").draw_cnt, GL_UNSIGNED_SHORT, 0);

	// unbind VAO and unload shader program
	glBindVertexArray(0);
	AssetManager::shaderval("image").UnUse();

}

/*  _________________________________________________________________________ */
/*
* a function to ndc position
* @param position of the game
* */

Vec2 GLApp::game_to_ndc(Vec2 position)
{
	Vec2 result;
	result.x = position.x * 2.0f / window->width_init;
	result.y = position.y * 2.0f / window->height_init;

	return result;
}

float GLApp::game_to_ndc(float position)
{
	return position * 2.0f / window->width_init;
}

