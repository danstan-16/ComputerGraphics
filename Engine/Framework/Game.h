#pragma once
#include <iostream>

namespace nu
{
	class Scene;

	class Game
	{
	public:
		Game() = default;
		virtual ~Game() = default;
		Game(Scene* scene) :
			m_scene{ scene }
		{}

		virtual bool Initialize() { return true; }
		virtual void Shutdown() {}

		virtual void Update(float dt) = 0;
		virtual void Draw(class Renderer& renderer) = 0;

		void SetScene(std::unique_ptr<Scene> scene) { m_scene = std::move(scene); }

	protected:
		std::unique_ptr<Scene> m_scene = std::make_unique<Scene>();
	};
}