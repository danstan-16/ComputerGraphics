#pragma once
#include "Framework\Actor.h"

namespace nu
{
    class PlayerController : public Actor
    {
    public:
        CLASS_PROTOTYPE(PlayerController)

        void Start() override;
        void Update(float dt) override;

        void OnCollision(Actor* other) override;

        void DestroyActor(Actor* other);

        void Read(const json::value_t& value) override;

    protected:
        class PhysicsComponent* m_physicsComponent{ nullptr };
        class SpriteAnimatorRendererComponent* m_rendererComponent{ nullptr };

        bool fireArrow = false;

        float m_deathTimer;
        bool m_timerOn = false;
        Actor* m_otherCollider;
    };
}


