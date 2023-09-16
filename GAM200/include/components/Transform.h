#include "Vec2.h"
#include "Composition.h"

	///The transform component provides a shared position and rotation.
	class Transform : public GameComponent
	{
	public:
		Transform();
		Vec2 Position;
		float Rotation;
		void Serialize(ISerializer& str);
	};