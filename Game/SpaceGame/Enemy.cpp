#include "pch.h"
#include "Enemy.h"
#include "Renderer/Renderer.h"
#include "Engine.h"
#include "Player.h"
#include "SpaceGame.h"
#include "Core/Factory.h"

FACTORY_REGISTER(Enemy)

void Enemy::Update(float dt)
{

	Player* player = m_scene->GetActorByName<Player>("PlayerPrototype");
	if (player)
	{
		nu::Vector2 direction = player->GetTransform().position - m_transform.position;
		float rotation = direction.Angle();
		SetRotation(rotation * nu::RadToDeg);

		nu::Vector2 forward{ 1, 0 };
		forward = forward.Rotate(m_transform.rotation * nu::DegToRad);
		SetVelocity(forward * m_speed);
	}

	Actor::Update(dt);

}

void Enemy::OnCollision(Actor* other)
{

	if (other->GetTag() == "PlayerBullet")
	{
		nu::Engine::Get().GetAudio().PlaySound("explosion");
		SetDestroyed();
		other->SetDestroyed();

		((SpaceGame*)m_scene->GetGame())->AddPoints(100);
	}

	for (int i = 0; i < 100; i++)
	{
		nu::Particle particle;
		particle.position = m_transform.position;
		particle.color = { 1.0f, 1.0f, 1.0f };
		particle.lifespan = nu::RandomFloat(0.5f, 2.0f);
		particle.velocity = { nu::RandomFloat(-600.0f, 600.0f), nu::RandomFloat(-600.0f, 600.0f) };

		nu::Engine::Get().GetPS().AddParticle(particle);
	}
}

void Enemy::Read(const nu::json::value_t & value)
{
	Actor::Read(value);

	JSON_READ_NAME(value, "speed", m_speed);
}
