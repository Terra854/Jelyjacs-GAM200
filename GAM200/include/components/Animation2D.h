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
class Animation2D : public Component
{
	public:
	Animation2D() : Component() {};

	std::map<AnimationType, std::vector<GLuint>> animation;

};