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
	Teleport,
	Idle_left,
	Push_left,
	Jump_left,
	Run_left,
	Teleport_left,
	No_Animation_Type // This should always be the last type
};

std::string AnimationTypeToString(AnimationType type);

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

	// Use false when setting up from prefabs, else use true when setting up map
	void set_up_map(bool val);
	

	AnimationType current_type=AnimationType::Idle;
	AnimationType previous_type=AnimationType::Idle;
	bool face_right = true;
	bool jump_fixed = false;
	bool reverse = false;

	

	bool fixed = false;
	bool invert = false;

	bool pause = false;
	
	float frame_count{};
	int frame_num{};
	
	
	GLApp::GLModel setup_texobj_animation(float x, float y, float z, float w, bool right);
	
	// animation data for level editor
	// all the animation data is stored in the map
	
	//  tpye -> model
	std::map<AnimationType, std::vector<GLApp::GLModel>> animation_Map;

	// @ yee ann these thing should can be edited in the level editor
	// the frame rate of the animation
	float frame_rate{};
	// the animation tile sheet
	GLuint animation_tex_obj;

	// base the tile sheet we get the number of row and col
	std::pair<float, float> animation_scale; // scale of the animation (col, row)

	// for each row we need to know the animation type and number of frame
	std::map<int, std::pair<int, AnimationType> > animation_frame; // [row] -> (frame, type)
	// @Guo Chen @Sen Chuan Need you to refactor animation_frame to this
	// std::map< AnimationType, std::pair<float, float> > animation_frame; // [type] -> (row, frame)

	// if we have jump animation type we need to know the fixed frame
	int jump_fixed_frame = 0;
	

	

	float opacity = 1.f;

	virtual ComponentType TypeId() const override { return ComponentType::Animation; }
};

AnimationType stringToAnimationType(const std::string& str);

bool animationIsLeft(AnimationType type);

