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
	Run,
	Jump
};
class Animation : public Component
{
public:
	Animation();
	void Initialize() override;
	GLuint animation_tex_obj;
	GLApp::GLModel setup_texobj_animation(float x, float y, float z, float w, bool right);
	std::map<AnimationType, std::vector<glm::vec4>> frame_map;
	std::map<AnimationType, std::vector<GLApp::GLModel>> animation_Map;
};