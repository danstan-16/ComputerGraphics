#pragma once
#include "Framework\Actor.h"

namespace nu
{
    class FlyingEnemyController : public Actor
    {
    public:
        CLASS_PROTOTYPE(FlyingEnemyController)

        void Start() override;
        void Update(float dt) override;

        void OnCollision(Actor* other) override;

        void Read(const json::value_t& value) override;

    protected:
        class PhysicsComponent* m_physicsComponent{ nullptr };
        class SpriteAnimatorRendererComponent* m_rendererComponent{ nullptr };
        float m_speed = 0.0f;
    };
}
