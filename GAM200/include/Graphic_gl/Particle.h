#include "glapp.h"
#include <Factory.h>
#include <Camera.h>
#include <components/Transform.h>
#include <components/Physics.h>
#include <Mat3.h>
#include <Vec2.h>

class ParticleSystem
{
public:
	ParticleSystem() {};
	~ParticleSystem() {};

	void Init();
	void Update();
	void Draw();
	void Free();
private:
	bool draw_particle = false;
	glm::vec2 translations[100]{};
	GLuint		instanceVBO{}, quadVAO{}, quadVBO{};
	Mat3 world_to_ndc{};
};