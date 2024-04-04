/* !
@file Animation.cpp
@author Yeo Jia Ming, Tay Sen Chuan, Guo Chen
@date	3/11/2023

This file contains the definition of the animation game component class
*//*__________________________________________________________________________*/
#include "../include/components/Animation.h"

std::string AnimationTypeToString(AnimationType type) {
	switch (type) {
		case AnimationType::Idle:
			return "Idle";
			break;
		case AnimationType::Push:
			return "Push";
			break;
		case AnimationType::Jump:
			return "Jump";
			break;
		case AnimationType::Run:
			return "Run";
			break;
		case AnimationType::Teleport:
			return "Teleport";
			break;
		case AnimationType::Idle_left:
			return "Idle_left";
			break;
		case AnimationType::Push_left:
			return "Push_left";
			break;
		case AnimationType::Jump_left:
			return "Jump_left";
			break;
		case AnimationType::Run_left:
			return "Run_left";
			break;
		case AnimationType::Teleport_left:
			return "Teleport_left";
			break;
		case AnimationType::No_Animation_Type:
			return "No_Animation_Type";
			break;		
	}
}

Animation::Animation()
	: Component(), animation_tex_obj{}
{}

void Animation::Initialize()
{

}

void Animation::Update_player()
{
	static float accum_time = 0.0f;
	static int frame_dt_count = 0;

	if (this->current_type != this->previous_type && !this->jump_fixed)
		this->frame_num = 0;
	if (engine->isPaused())
		return;

	/*
	accum_time += engine->GetDt();

	if (accum_time < engine->Get_Fixed_DT()) return;

	while (accum_time >= engine->Get_Fixed_DT())
	{
		accum_time -= engine->Get_Fixed_DT();
		frame_dt_count++;
	}
	*/

	frame_dt_count = engine->Get_NumOfSteps();
	

	while (frame_dt_count) {
		frame_dt_count--;
		if (!pause) {
			if (this->current_type != this->previous_type && !this->jump_fixed)
				this->frame_num = 0;
			else if (this->frame_count >= this->frame_rate) {
				this->frame_count = 0.f;
				this->reverse ? this->frame_num-- : this->frame_num++;
				if (this->frame_num >= this->animation_Map[this->current_type].size())
					this->frame_num = 0;
				else if (this->frame_num < 0)
					this->frame_num = static_cast<int>(this->animation_Map[this->current_type].size()) - 1;
			}
			if (this->jump_fixed) {
				if (this->previous_type != AnimationType::Jump && this->previous_type != AnimationType::Jump_left)
					this->frame_num = 0;
				if (this->frame_num >= this->jump_fixed_frame)
					this->frame_num = this->jump_fixed_frame;
			}
		}
	}
}

void Animation::Update_objects()
{
	static float accum_time = 0.0f;
	static int frame_dt_count = 0;

	if (this->current_type != this->previous_type)
		this->frame_num = 0;
	if (engine->isPaused())
		return;

	/*
	accum_time += engine->GetDt();
	
	if (accum_time < engine->Get_Fixed_DT()) return;

	while (accum_time >= engine->Get_Fixed_DT())
	{
		accum_time -= engine->Get_Fixed_DT();
		frame_dt_count++;
	}
	*/

	frame_dt_count = engine->Get_NumOfSteps();
	

	while (frame_dt_count) {
		frame_dt_count--;
		if (!pause) {
			if (!this->fixed) {//object with animation
				if (this->frame_count >= this->frame_rate)
				{
					this->frame_count = 0.f;
					this->frame_num++;
					if (this->frame_num >= this->animation_Map[this->current_type].size())
						this->frame_num = 0;
				}
			}
			else if (this->frame_count >= this->frame_rate) {
				this->frame_count = 0.f;

				this->reverse ? this->frame_num-- : this->frame_num++;

				if (this->frame_num < 0)
					this->frame_num = 0;
				else if (this->frame_num >= this->animation_Map[this->current_type].size())
					this->frame_num = static_cast<int>(this->animation_Map[this->current_type].size()) - 1;
			}
		}
	}
}

void Animation::Update_time()
{
	this->previous_type = this->current_type;
	static float accum_time = 0.0f;
	static int frame_dt_count = 0;

	if (engine->isPaused())
		return;

	/*
	accum_time += engine->GetDt();
	
	if (accum_time < engine->Get_Fixed_DT()) return;

	while (accum_time >= engine->Get_Fixed_DT())
	{
		accum_time -= engine->Get_Fixed_DT();
		frame_dt_count++;
	}
	*/

	frame_dt_count = engine->Get_NumOfSteps();

	while (frame_dt_count) {
		frame_dt_count--;
		
		this->frame_count += engine->Get_Fixed_DT();
	}
}

int Animation::get_ani_type_count()
{
	int counter{};
	// check through animation_Map
	for (auto& it : this->animation_Map)
	{
		(void)it;
		counter++;
	}
	return counter;
}

// Get all the models and properly store them into the map to be rendered
void Animation::set_up_map()
{
	frame_num = 0;
	animation_Map.clear(); // Clear any existing map before setting up

	//loop through the animation_frame map; frame.second.first = framecol = i, frame.first = framerow
	for (auto& frame : this->animation_frame)
	{
		// Check if there is any error with animations
		auto it = animation_Map.find(frame.second.second);

		if (frame.second.second == AnimationType::No_Animation_Type)
		{
			std::cout << "Something went wrong while setting up animations!\n";
			return;
		}

		bool faceright = true;
		if (animationIsLeft(frame.second.second))
		{
			faceright = false;
		}

		std::vector<GLApp::GLModel> animationmodel;

		for (float i = 0; i < frame.second.first; i++)
		{
			float g = static_cast<float>(frame.first);
			// The special cases
			if (g > animation_scale.second && faceright == false)
				g -= animation_scale.second;
			else if (g > animation_scale.second && frame.second.second == AnimationType::Idle)
				g = 1;
			else if (g > animation_scale.second)
				g = animation_scale.second;

			GLApp::GLModel model = setup_texobj_animation((i / animation_scale.first), ((g - 1) / animation_scale.second), ((i + 1) / animation_scale.first), (g / animation_scale.second), faceright);

			animationmodel.push_back(model);
		}

		// Check if animation type already exist
		if (it != animation_Map.end()) 
		{
			// Same type exist, append the vectors
			animation_Map[frame.second.second].insert(animation_Map[frame.second.second].end(), animationmodel.begin(), animationmodel.end());
		}
		else // Add to animation_Map
		{
			animation_Map.emplace(frame.second.second, animationmodel);
		}
	}

	// Info dump
	for (auto& it : animation_Map)
	{
		std::cout << "Animation Type Setup: " << it.first << " Size is: " << it.second.size() << std::endl;
	}
}



/******************************************************************************
	to create a model
*******************************************************************************/
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

	//std::cout << "frame created" << std::endl;
	return Model;
}

// Converts string to animationType
AnimationType stringToAnimationType(const std::string& str) {
	static const std::map<std::string, AnimationType> stringToType =
	{
		{"idle", AnimationType::Idle},
		{"push", AnimationType::Push},
		{"run", AnimationType::Run},
		{"jump", AnimationType::Jump},
		{"teleport", AnimationType::Teleport}
	};
	auto it = stringToType.find(str);
	if (it != stringToType.end())
		return it->second;
	else
		return AnimationType::No_Animation_Type; // Default value
}

bool animationIsLeft(AnimationType type)
{
	switch (type)
	{
	case AnimationType::Idle_left:
		return true;
		break;
	case AnimationType::Push_left:
		return true;
		break;
	case AnimationType::Jump_left:
		return true;
		break;
	case AnimationType::Run_left:
		return true;
		break;
	case AnimationType::Teleport_left:
		return true;
		break;
	default:
		return false;
		break;
	}
}


