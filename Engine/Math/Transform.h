#pragma once
#include "Serialization/Json.h"
#include "Editor/Editor.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/transform.hpp>

namespace nu
{
	struct Transform : public IGUI
	{
		glm::vec3 position{ 0.0f };
		glm::vec3 rotation{ glm::identity<glm::quat>() }; // degrees
		glm::vec3 scale{ 1.0f };

		void SetRotation(const glm::vec3& euler) { rotation = glm::quat(glm::radians()) }
		void SetRoation(const glm::quat& qrotation) { rotation = glm::normalize(qrotation); }
		glm::vec3 GetRotation() const { return glm::degrees(glm::eulerAngles(rotation)); }

		const glm::mat4& GetMatrix()
		{
			auto mxt = glm::translate(position);
			auto mxr = glm::eulerAngleYXZ(
				glm::radians(rotation.y), 
				glm::radians(rotation.x), 
				glm::radians(rotation.z));

			auto mxs = glm::scale(scale);

			auto matrix = mxt * mxr * mxs;

			return matrix;
		}

		void Read(const json::value_t& value)
		{
			//JSON_READ(value, position);
			//JSON_READ(value, rotation);
			//JSON_READ(value, scale);
		}
	};
}