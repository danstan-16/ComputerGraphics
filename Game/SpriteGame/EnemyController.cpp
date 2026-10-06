#include "EnemyController.h"
#include "Components/PhysicsComponent.h"
#include "Components/SpriteAnimatorRendererComponent.h"
#include "SpriteGame.h"
#include "Engine.h"

namespace nu
{
	FACTORY_REGISTER(EnemyController)

	void EnemyController::Start()
	{
		Actor::Start();

		m_physicsComponent = GetComponent<PhysicsComponent>();
		assert(m_physicsComponent);
		m_rendererComponent = GetComponent<SpriteAnimatorRendererComponent>();
		assert(m_rendererComponent);

	}

	void EnemyController::Update(float dt)
	{
		if (!m_physicsComponent) Start();
		Vector2 velocity = m_physicsComponent->GetVelocity();

		float dir = 0.0f;
		auto player = m_scene->GetActorByName("PlayerPrototype");
		if (player)
		{
			Vector2 position = GetTransform().position;
			Vector2 playerPosition = player->GetTransform().position;

			if (playerPosition.x < position.x) dir = -1.0f;
			else dir = +1.0f;
		}

		if (m_playAttack)
		{
			m_rendererComponent->Play("attack");
		}
		else if (dir != 0.0f)
		{
			velocity.x = dir * 100;
			m_rendererComponent->Play("run");
		}
		else
		{
			m_rendererComponent->Play("idle");
		}
		

		m_rendererComponent->SetFlipH(dir < 0.0f);

		m_physicsComponent->SetVelocity(velocity);

		Actor::Update(dt);
	}

	void EnemyController::OnCollision(Actor* other)
	{
		if (other->GetTag() == "PlayerArrow")
		{
			SetDestroyed(true);
			other->SetDestroyed(true);
			((SpriteGame*)m_scene->GetGame())->AddPoints(100);
		}

		if (other->GetTag() == "Player")
		{
			m_playAttack = true;
			Update(Engine::Get().GetTime().GetDeltaTime());
		}
	}

	void EnemyController::Read(const json::value_t& value)
	{
		Actor::Read(value);

		JSON_READ_NAME(value, "speed", m_speed);
	}
}