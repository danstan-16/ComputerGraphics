#pragma once
#include <iostream>
#include "Math/Vector2.h"
#include "Resources/Resource.h"

struct SDL_Texture;

namespace nu
{
	class Texture : public Resource
	{
	public:
		Texture() = default;
		~Texture();

		bool Load(const std::string& filename, class Renderer& renderer);

		Vector2 GetSize() const
		{
			return m_size;
		}

		friend class Renderer;
		SDL_Texture* m_texture{ nullptr };

	private:
		Vector2 m_size{ 0.0f, 0.0f };
	};
}
