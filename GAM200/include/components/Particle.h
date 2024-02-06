#pragma once
/* !
@file Particle.h
@author Yeo Jia Ming, Tay Sen Chuan, Guo Chen
@date	3/11/2023

This file contains the declaration for the Particle game component class
*//*__________________________________________________________________________*/
#include <Debug.h>
#include <string>
#include <vector>
#include "Object.h"
#include "../../src/Assets Manager/serialization.h"
#include "../Graphic_gl/glapp.h"

#define PARTICLE_NUM 100

class ParticleSystem : public Component
{
public:

	ParticleSystem() {};
	~ParticleSystem() {};
	void Initialize() override {};


	class Particle
	{
	public:
		Particle(Vec2* pos_in) : position(pos_in) {};
		~Particle() {};

		//void Init();
		void Update();


		Vec2* position{};
		Vec2 velocity{};
		float life_time{};
		//float size{};
		float life_count{};
		//bool active{};
	};

	

		void Init();
		void Update();
		void Draw();
		void Free();

		std::vector<std::unique_ptr<Particle>> particles{};
		bool draw_particle = false;
		Vec2 translations[PARTICLE_NUM]{};
		GLuint		instanceVBO{}, quadVAO{}, quadVBO{};
		Mat3 world_to_ndc{};


		// texture
		GLuint particle_texture= GLApp::setup_texobj("Asset/Picture/p2.png");

		// position
		float pos_x_min = 1.6f;
		float pos_x_max = 1.6f;
		float pos_y_min = -0.5f;
		float pos_y_max = 0.5f;

		// velocity
		float vel_x_min = -1.0f;
		float vel_x_max = -2.0f;
		float vel_y_min = 0.0f;
		float vel_y_max = 0.0f;

		// life time
		float life_min = 0.7f;
		float life_max = 2.0f;

	

	virtual ComponentType TypeId() const override { return ComponentType::ParticleSystem; }
};

Vec2 random_position(float x_min, float x_max, float y_min, float y_max);
Vec2 random_velocity(float x_min, float x_max, float y_min, float y_max);
float random_life_time(float min, float max);

