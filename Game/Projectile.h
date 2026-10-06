#pragma once
#include "Framework\Actor.h"

struct ProjectileDesc : public nu::ActorDesc {
	float speed;
};

namespace nu
{
    class Projectile : public Actor
    {
		Projectile() = default;
		Projectile(const ProjectileDesc& ProjectileDesc) :
			Actor(ProjectileDesc),
			m_speed{ ProjectileDesc.speed }
		{
		}

		CLASS_PROTOTYPE(Projectile)

			void Update(float dt) override;

		void Read(const nu::json::value_t& value) override;

	private:
		float m_speed = 800.0f;
    };
}


