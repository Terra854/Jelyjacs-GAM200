#include "Vec2.h"
#include "Composition.h"

	class Transform : public GameComponent
	{
	public:
		//centre position of bodies
		Vec2 Position;
		Vec2 PrevPosition;
		float Velocity;
		float Acceleration;
		float Mass;
	};