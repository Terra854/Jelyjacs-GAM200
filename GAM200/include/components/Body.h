
#include "Composition.h"
#include "Collision.h"
#include "Transform.h"

	class Body : public GameComponent
	{
	public:
		Body();
		~Body();

		void AddForce(Vec2 force);
		void Integrate(float dt);
		void SetPosition(Vec2);
		void SetVelocity(Vec2);
		void PublishResults();

		//Draw the object using the debug drawer
		void DebugDraw();

		virtual void Initialize();
		virtual void Serialize(ISerializer& stream);

		Vec2 Position;
		Vec2 PrevPosition;
		Vec2 Velocity;
		Vec2 Acceleration;
		float Mass;


		//Transform for this body
		Transform* tx;
		//Static object are immovable fixed objects
		bool IsStatic;


	};