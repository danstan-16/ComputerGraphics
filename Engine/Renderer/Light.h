#pragma once
#include <glm/glm.hpp>
#include <cstdint>

namespace nu
{
	constexpr uint32_t MAX_LIGHTS = 8;

	enum class LightType : uint32_t
	{
		Directional = 0,
		Point = 1,
		Spot = 2
	};

	struct LightData
	{
		glm::vec3 position;
		LightType type;
		glm::vec3 direction;
		float range;
		glm::vec3 color;
		float intensity;
		float innerCutoff;
		float outerCutoff;
		glm::vec3 padding;
	};

	struct LightUniform
	{
		LightData lights[MAX_LIGHTS];
		glm::vec3 ambient;
		uint32_t count;
	};
}