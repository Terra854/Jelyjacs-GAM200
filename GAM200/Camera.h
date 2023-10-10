#include <Mat3.h>
#include <Vec2.h>
#include <interface_System.h>
class Camera : public ISystems
{
	Camera();
	~Camera() {}
	Vec2 position;
	Vec2 scale;
	Mat3 world_to_ndc;

	virtual void Initialize();
	virtual void Update();

};
extern Camera* camera;