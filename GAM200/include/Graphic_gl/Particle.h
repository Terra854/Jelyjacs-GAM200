#pragma once
/* !
@file    Particle.h
@author  Guo Chen (g.chen@digipen.edu)
@date    02/11/2023

Header file for Particle.cpp
*//*__________________________________________________________________________*/
#include "glapp.h"
#include <Factory.h>
#include <Camera.h>
#include <components/Transform.h>
#include <components/Physics.h>
#include <Mat3.h>
#include <Vec2.h>
#include <vector>

#define PARTICLE_NUM 100

class Particle
{
public:
	Particle(Vec2* pos_in) : position(pos_in), velocity(1.0f, 0.0f), life_time(2.0f), size(0.1f), life_count(0.0f),active(true) {};
	~Particle() {};

	//void Init();
	void Update();


	Vec2* position{};
	Vec2 velocity{};
	float life_time{};
	float size{};
	float life_count{};
	bool active{};
};

class ParticleSystem
{
public:
	ParticleSystem() {};
	~ParticleSystem() {};

	void Init();
	void Update();
	void Draw();
	void Free();

	std::vector<std::unique_ptr<Particle>> particles{};
	bool draw_particle = false;
	Vec2 translations[PARTICLE_NUM]{};
	GLuint		instanceVBO{}, quadVAO{}, quadVBO{};
	Mat3 world_to_ndc{};
	GLuint particle_texture{};

	// position
	float pos_x_min = 1.0f;
	float pos_x_max = 1.0f;
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

	
};
Vec2 random_position(float x_min, float x_max, float y_min, float y_max);
Vec2 random_velocity(float x_min, float x_max, float y_min, float y_max);
float random_life_time(float min, float max);