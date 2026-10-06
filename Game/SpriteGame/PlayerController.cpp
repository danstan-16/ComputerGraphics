#include "PlayerController.h"
#include "Components/PhysicsComponent.h"
#include "Components/SpriteAnimatorRendererComponent.h"
#include "SpriteGame.h"
#include "Projectile.h"
#include "Engine.h"
#include "EnemyController.h"

#include <thread>
#include <chrono>

namespace nu
{
	FACTORY_REGISTER(PlayerController)

	void PlayerController::Start()
	{
		Actor::Start();

		m_physicsComponent = GetComponent<PhysicsComponent>();
		assert(m_physicsComponent);
		m_rendererComponent = GetComponent<SpriteAnimatorRendererComponent>();
		assert(m_rendererComponent);

	}

	void PlayerController::Update(float dt)
	{
		if (!m_physicsComponent) Start();
		Vector2 velocity = m_physicsComponent->GetVelocity();

		float dir = 0.0f;
		if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_A)) dir = -1.0f;
		if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_D)) dir = +1.0f;

		if (Engine::Get().GetInput().GetKeyPressed(SDL_SCANCODE_SPACE))
		{
			velocity.y = -800.0f;
		}

		if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_RETURN))
		{
			m_rendererComponent->Play("attack");

			std::this_thread::sleep_for(std::chrono::milliseconds(200));

			if (!fireArrow)
			{
				nu::Engine::Get().GetAudio().PlaySound("shoot");
				auto arrow = nu::Factory::Instance().Create<Actor>("ProjectilePrototype");

				arrow->SetTransform(m_transform);
				arrow->SetScale(3.0f);
				arrow->SetTag("PlayerArrow");


				if (dir < 0.0f)
				{
					arrow->GetComponent<SpriteRendererComponent>()->SetFlipH(true);
					arrow->SetRotation(180);
				}

				m_scene->AddActor(std::move(arrow));
				fireArrow = true;
			}

		}
		else if (Engine::Get().GetInput().GetKeyReleased(SDL_SCANCODE_RETURN))
		{
			fireArrow = false;
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
		Engine::Get().GetRenderer().SetCamera(m_physicsComponent->GetPosition());

		if (m_timerOn)
		{
			m_deathTimer -= dt;
			if (m_deathTimer <= 0.0f)
			{
				DestroyActor(m_otherCollider);
			}
		}

		Actor::Update(dt);
	}

	void PlayerController::OnCollision(Actor* other)
	{
		if (other->GetTag() == "Enemy" )
		{
			other->OnCollision(this);
			m_deathTimer = 0.6f;
			m_timerOn = true;
			m_otherCollider = other;
		}
	}

	void PlayerController::DestroyActor(Actor* other)
	{
		SetDestroyed(true);
		other->SetDestroyed(true);
		((SpriteGame*)m_scene->GetGame())->OnPlayerDead();
	}

	void PlayerController::Read(const json::value_t& value)
	{
		Actor::Read(value);
	}
}