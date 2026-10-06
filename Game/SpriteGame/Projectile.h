#pragma once
#include "Framework\Actor.h"


namespace nu
{
	struct ProjectileDesc : public nu::ActorDesc {
		float speed;
	};

    class Projectile : public Actor
    {
	public:
		Projectile() = default;
		Projectile(const ProjectileDesc& ProjectileDesc) :
			Actor(ProjectileDesc),
			m_speed{ ProjectileDesc.speed }
		{
		}

		CLASS_PROTOTYPE(Projectile)

		void Update(float dt) override;

		void Read(const json::value_t& value) override;

	private:
		float m_speed = 800.0f;
    };
}


