#pragma once
/* !
@file Animation.h
@author Yeo Jia Ming, Tay Sen Chuan, Guo Chen
@date	3/11/2023

This file contains the declaration for the animation game component class
*//*__________________________________________________________________________*/
#include <Debug.h>
#include <string>
#include <vector>
#include "Object.h"
#include "../../src/Assets Manager/serialization.h"
#include "../Graphic_gl/glapp.h"

enum AnimationType
{
	Idle,
	Push,
	Jump,
	Run,
	Idle_left,
	Push_left,
	Jump_left,
	Run_left,
	Error // This should always be the last type
};

class Animation : public Component
{
public:
	Animation();
	void Initialize() override;

	void Update_player();
	void Update_objects();
	void Update_time();

	void draw();

	int get_ani_type_count();
	void set_up_map();
	

	AnimationType current_type=AnimationType::Idle;
	AnimationType previous_type=AnimationType::Idle;
	bool face_right = true;
	bool jump_fixed = false;
	bool reverse = false;

	int jump_fixed_frame = 0;

	bool fixed = false;
	bool invert = false;
	
	float frame_count{};
	int frame_num{};
	
	GLuint animation_tex_obj;
	GLApp::GLModel setup_texobj_animation(float x, float y, float z, float w, bool right);
	
	// animation data
	std::map<AnimationType, std::vector<GLApp::GLModel>> animation_Map;


	float frame_rate{};
	std::pair<float, float> animation_scale; // scale of the animation (col, row)

	std::map< float, std::pair<float, AnimationType> > animation_frame; // [row] -> (frame, type)

	float opacity = 1.f;

	virtual ComponentType TypeId() const override { return ComponentType::Animation; }
};

AnimationType stringToAnimationType(const std::string& str);

bool animationIsLeft(AnimationType type);

