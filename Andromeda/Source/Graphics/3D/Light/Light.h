#pragma once

#include <glm/glm.hpp>

namespace AD {
	class Light {
	private:
		glm::vec3 m_Translation;
		glm::vec3 m_Rotation;
	public:
		Light();
		~Light();

		void SetTranslation(const glm::vec3& translation);
		void SetRotation(const glm::vec3& rotation);

		const glm::vec3& GetTranslation();
		const glm::vec3& GetRotation();

		glm::vec3& GetTranslationRef();
		glm::vec3& GetRotationRef();

		static Light* DeepCopy(Light* light);
	};
}