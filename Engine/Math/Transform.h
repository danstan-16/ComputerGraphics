#pragma once
#include "Serialization/Json.h"
#include "Vector2.h"

namespace nu
{
	struct Transform
	{
		Vector2 position;
		float rotation;
		float scale;

		void Read(const json::value_t& value)
		{
			JSON_READ(value, position);
			JSON_READ(value, rotation);
			JSON_READ(value, scale);
		}
	};
}