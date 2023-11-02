/* !
@file	Animation.cpp
@author	Guo Chen
@date	02/11/2023

This file contains the implementation for the Animation component class
*//*__________________________________________________________________________*/

#include "../include/components/Animation.h"

Animation::Animation()
	: Component(), animation_tex_obj{}
{}

void Animation::Initialize()
{

}

GLApp::GLModel Animation::setup_texobj_animation(float x, float y, float z, float w, bool right)
{
	GLApp::GLModel Model;
	GLuint vbo, vao, ebo;
	std::vector < float > pos_vtx;
	std::vector < float > clr_vtx;
	std::vector < float > tex_coor;
	std::vector < GLushort > gl_tri_primitives;

	// tex_coor

	glm::vec2 tex_coor_data[4] = {
		glm::vec2(z, y),
		glm::vec2(z, w),
		glm::vec2(x, w),
		glm::vec2(x, y)
	};
	for (int i = 0; i < 4; i++) {
		tex_coor.push_back(tex_coor_data[i].x);
		tex_coor.push_back(tex_coor_data[i].y);
	}
	// pos_vtx
	glm::vec2 pos_vtx_data[4] = {
		glm::vec2(right ? 1.0f : -1.0f , 1.0f),
		glm::vec2(right ? 1.0f : -1.0f , -1.0f),
		glm::vec2(right ? -1.0f : 1.0f , -1.0f),
		glm::vec2(right ? -1.0f : 1.0f , 1.0f)
	};
	for (int i = 0; i < 4; i++) {
		pos_vtx.push_back(pos_vtx_data[i].x);
		pos_vtx.push_back(pos_vtx_data[i].y);
	}
	// tri_primitives
	GLushort glushort_data[6] = {
		0, 1, 2,
		2, 3, 0
	};
	for (int i = 0; i < 6; i++) {
		gl_tri_primitives.push_back(glushort_data[i]);
	}

	GLApp::GLModel model;
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
	Model.primitive_type = GL_TRIANGLES;

	std::cout << "frame created" << std::endl;
	return Model;
}