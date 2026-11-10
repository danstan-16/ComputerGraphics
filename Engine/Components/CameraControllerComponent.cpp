#include "pch.h"
#include "CameraControllerComponent.h"
#include "Engine.h"

namespace nu
{
	//void nu::CameraControllerComponent::Update(float dt)
	//{
//		// get camera rotation
//		if (Engine::Get().GetInput().GetMouseButtonDown(InputSystem::MouseButton::Right))
//		{
//			glm::vec2 axis = GetEngine().GetInput().GetMouseDelta() * sensitivity;
//
//			yaw -= axis.x;
//			pitch -= axis.y;
//			pitch = glm::clamp(pitch, -89.0f, 89.0f);
//		}
//
//		//owner->transform.SetRotation(glm::vec3(pitch, yaw, 0.0f));
//
//		glm::quat q = glm::quat_cast(glm::eulerAngleYXZ(glm::radians(yaw), glm::radians(pitch), 0.0f));
//		owner->transform.SetRotationQuat(q);
//
//		glm::vec3 direction{ 0 };
//		if (GetEngine().GetInput().GetKeyDown(SDL_SCANCODE_A)) direction -= owner->transform.Right();
//		if (GetEngine().GetInput().GetKeyDown(SDL_SCANCODE_D)) direction += owner->transform.Right();
//		if (GetEngine().GetInput().GetKeyDown(SDL_SCANCODE_E)) direction += owner->transform.Up();
//		if (GetEngine().GetInput().GetKeyDown(SDL_SCANCODE_Q)) direction -= owner->transform.Up();
//		if (GetEngine().GetInput().GetKeyDown(SDL_SCANCODE_W)) direction += owner->transform.Forward();
//		if (GetEngine().GetInput().GetKeyDown(SDL_SCANCODE_S)) direction -= owner->transform.Forward();;
//
//		if (glm::length2(direction) > 0.0f) {
//			direction = glm::normalize(direction);
//		}
//		owner->transform.position += direction * (speed * dt);
//
//	}
}