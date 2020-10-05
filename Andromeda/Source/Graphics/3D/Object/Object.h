#pragma once

#include "Mesh3D.h"
#include "Texture.h"
#include "Shader.h"

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include <memory>
#include <string>

namespace AD {
	class Object {
	private:
		std::shared_ptr<Mesh3D> m_Mesh;
		std::shared_ptr<Texture> m_Texture;
		std::shared_ptr<Shader> m_Shader;

		glm::vec3 m_Rotation;
		glm::vec3 m_Translation;
		glm::vec3 m_Scale;

		bool m_IsDynamic;

		glm::mat4 m_FinalTransformation;

	public:
		Object(const std::string& meshPath, const std::string& shaderPath, const std::string& texturePath, bool isDynamic);
		~Object();

		const glm::vec3& GetTranslation();
		const glm::vec3& GetRotation();
		const glm::vec3& GetScale();
		glm::vec3& GetTranslationRef();
		glm::vec3& GetRotationRef();
		glm::vec3& GetScaleRef();

		Mesh3D* GetMesh();
		Shader* GetShader();
		Texture* GetTexture();

		void SetTranslation(const glm::vec3& translation);
		void SetRotation(const glm::vec3& rotation);
		void SetScale(const glm::vec3& scale);

		void AddTranslation(const glm::vec3& translation);
		void AddRotation(const glm::vec3& rotation);
		void AddScale(const glm::vec3& scale);

		glm::mat4 GetTransformationMatrix();
		void LookAtPosition(const glm::vec3& position);
		void LookInDirection(const glm::vec3& forwardDirection);
		void LookInDirectionX(const glm::vec3& forwardDirection);

		void GenerateFinalTransformation(const glm::mat4& offset);
		void SetFinalTransformation(const glm::mat4& transformation);
		void Draw(const glm::mat4& projection, const glm::mat4& view);
		void Draw(const glm::mat4& projection, const glm::mat4& view, const glm::mat4& modelOffset);

		static Object* DeepCopy(Object* object);
	};
}