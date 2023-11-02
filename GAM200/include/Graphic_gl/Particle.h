#include "glapp.h"

class ParticleSystem
{
public:
	ParticleSystem();
	~ParticleSystem();

	void Init();
	void Update(float dt);
	void Draw();

private:
	int particleCount{};
	glm:: vec2 position{};
	glm:: vec2 velocity{};
	GLenum		primitive_type{ 0 };
	GLuint		primitive_cnt{  };
	GLuint		vaoid{ 0 };
	GLuint		draw_cnt{ 0 };
	GLSLShader	shdr_pgm;
	GLuint 	    texobj{ 0 };
	const char* texfile{ nullptr };
};