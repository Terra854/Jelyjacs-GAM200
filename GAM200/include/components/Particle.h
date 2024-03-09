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
#include "../../src/Assets Manager/asset_manager.h"
#define PARTICLE_NUM 100

enum class PrticleType {
	Finn,

};

class ParticleSystem : public Component
{
public:

	ParticleSystem() {};
	~ParticleSystem() {};
	void Initialize() override { Init(); };


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
		void Update(Object* player);
		void Draw();
		void Free();

		std::vector<std::unique_ptr<Particle>> particles{};
		bool draw_particle = false;
		Vec2 translations[PARTICLE_NUM]{};
		GLuint		instanceVBO{}, quadVAO{}, quadVBO{};
		Mat3 world_to_ndc{};


		// texture
		GLuint particle_texture{};

		// position
		float pos_x_min{};
		float pos_x_max{};
		float pos_y_min{};
		float pos_y_max{};

		// velocity
		float vel_x_min{};
		float vel_x_max{};
		float vel_y_min{};
		float vel_y_max{};

		// life time
		float life_min{};
		float life_max{};

	

	virtual ComponentType TypeId() const override { return ComponentType::ParticleSystem; }
};

Vec2 random_position(float x_min, float x_max, float y_min, float y_max);
Vec2 random_velocity(float x_min, float x_max, float y_min, float y_max);
float random_life_time(float min, float max);

