#include "pch.h"
#include "Player.h"
#include "Renderer/Renderer.h"
#include "Engine.h"
#include "Assets.h"
#include "Bullet.h"
#include "SpaceGame.h"
#include "Renderer/Model.h"
#include "Resources/Resource.h"
#include "Core/Factory.h"
#include "Components/PhysicsComponent.h"

FACTORY_REGISTER(Player)

void Player::Update(float dt)
{
	// movement
	float thrust = 0.0f;
	if (nu::Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_W)) thrust = m_speed;
	if (nu::Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_S)) thrust = -m_speed;

	float rotate = 0.0f;
	if (nu::Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_A)) rotate = -180.0f;
	if (nu::Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_D)) rotate = +180.0f;

	nu::PhysicsComponent* physicsComponent = GetComponent<nu::PhysicsComponent>();
	if (physicsComponent)
	{
		nu::Vector2 forward{ 1, 0 }; // -->
		nu::Vector2 force = forward.Rotate(m_transform.rotation * nu::DegToRad) * thrust;
		
		physicsComponent->ApplyForce(force);
		physicsComponent->ApplyTorque(rotate);

		nu::Vector2 position = physicsComponent->GetPosition();

		nu::Engine::Get().GetRenderer().SetCamera(position);
	}

	// particle system
	if (thrust)
	{
		nu::Particle particle;

		nu::Vector2 offset{ -20.0f, 0.0f };
		offset = offset.Rotate(m_transform.rotation * nu::DegToRad);
		particle.position = m_transform.position + offset;

		particle.texture = nu::Resources().Get<nu::Texture>("textures/particle.png", nu::Engine::Get().GetRenderer());
		particle.lifespan = nu::RandomFloat(0.5f, 1.5f);
		particle.velocity = { nu::RandomFloat(-200.0f, 200.0f), nu::RandomFloat(-200.0f, 200.0f) };

		nu::Engine::Get().GetPS().AddParticle(particle);
	}
	


	//fire 
	if (nu::Engine::Get().GetInput().GetKeyPressed(SDL_SCANCODE_SPACE))
	{
		nu::Engine::Get().GetAudio().PlaySound("pewpew");

		auto bullet = nu::Factory::Instance().Create<Bullet>("BulletPrototype");
		bullet->SetTransform(m_transform);
		bullet->SetScale(2.0f);
		bullet->SetTag("PlayerBullet");

		m_scene->AddActor(std::move(bullet));
		
	}

	// bullet time
	if (nu::Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_X))
	{
		nu::Engine::Get().GetTime().SetTimeScale(0.5f);
	}
	else
	{
		nu::Engine::Get().GetTime().SetTimeScale(1.0f);
	}

	Actor::Update(dt);


}

void Player::OnCollision(Actor* other)
{
	if(other->GetTag() == "Enemy")
	{
		SetDestroyed();
		((SpaceGame*)m_scene->GetGame())->OnPlayerDead();
	}
}

void Player::Read(const nu::json::value_t& value)
{
	Actor::Read(value);

	JSON_READ_NAME(value, "speed", m_speed);
}