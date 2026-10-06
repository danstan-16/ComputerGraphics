#include "Projectile.h"
#include "Core/Factory.h"
#include "Math/MathUtils.h"
#include "Components/PhysicsComponent.h"

namespace nu
{
	FACTORY_REGISTER(Projectile)

	void Projectile::Update(float dt)
	{
		PhysicsComponent* physicsComponent = GetComponent<PhysicsComponent>();
		if (physicsComponent)
		{
			Vector2 forward{ 100, 0 }; // -->

			Vector2 velocity = forward.Rotate(m_transform.rotation * DegToRad);

			physicsComponent->SetVelocity(velocity);

			nu::Vector2 position = physicsComponent->GetPosition();
			physicsComponent->SetPosition(position);
		}

		Actor::Update(dt);
	}

	void Projectile::Read(const json::value_t& value)
	{
		Actor::Read(value);

		JSON_READ_NAME(value, "speed", m_speed);
	}
}