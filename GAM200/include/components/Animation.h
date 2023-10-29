#pragma once
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
	End // This should always be the last type
};
class Animation : public Component
{
public:
	Animation();
	void Initialize() override;

	AnimationType current_type=AnimationType::Idle;
	AnimationType previous_type=AnimationType::Idle;
	bool face_right = true;
	bool jump_fixed = false;


	int jump_fixed_frame = 0;

	bool fixed = false;
	bool invert = false;
	float frame_rate{};
	float frame_count{};
	int frame_num{};

	GLuint animation_tex_obj;
	GLApp::GLModel setup_texobj_animation(float x, float y, float z, float w, bool right);

	std::map<AnimationType, std::vector<GLApp::GLModel>> animation_Map;

	virtual ComponentType TypeId() const override { return ComponentType::Animation; }
};

