#include "Object.h"
#include "Mesh3DManager.h"
#include "TextureManager.h"
#include "ShaderManager.h"
#include "Console.h"

#include <glm/gtx/euler_angles.hpp>

namespace AD {
	Object::Object(const std::string& meshPath, const std::string& shaderPath, const std::string& texturePath, bool isDynamic)
		: m_Mesh(Mesh3DManager::GetInstance()->GetMesh(meshPath, isDynamic)), m_Shader(ShaderManager::GetInstance()->GetShader(shaderPath)), m_Texture(TextureManager::GetInstance()->GetTexture(texturePath)), m_Translation(0.0f), m_Rotation(0.0f), m_Scale(1.0f), m_IsDynamic(isDynamic), m_FinalTransformation(1.0f)
	{

	}

	Object::~Object()
	{
		Mesh3DManager::GetInstance()->MarkForCleanup();
		ShaderManager::GetInstance()->MarkForCleanup();
		TextureManager::GetInstance()->MarkForCleanup();
	}

	const glm::vec3& Object::GetTranslation()
	{
		return m_Translation;
	}

	const glm::vec3& Object::GetRotation()
	{
		return m_Rotation;
	}

	const glm::vec3& Object::GetScale()
	{
		return m_Scale;
	}

	glm::vec3& Object::GetTranslationRef()
	{
		return m_Translation;
	}

	glm::vec3& Object::GetRotationRef()
	{
		return m_Rotation;
	}

	glm::vec3& Object::GetScaleRef()
	{
		return m_Scale;
	}

	Mesh3D* Object::GetMesh()
	{
		return m_Mesh.get();
	}

	Shader* Object::GetShader()
	{
		return m_Shader.get();
	}

	Texture* Object::GetTexture()
	{
		return m_Texture.get();
	}

	void Object::SetTranslation(const glm::vec3& translation)
	{
		m_Translation = translation;
	}

	void Object::SetRotation(const glm::vec3& rotation)
	{
		m_Rotation = rotation;
	}

	void Object::SetScale(const glm::vec3& scale)
	{
		m_Scale = scale;
	}

	void Object::AddTranslation(const glm::vec3& translation)
	{
		m_Translation += translation;
	}

	void Object::AddRotation(const glm::vec3& rotation)
	{
		m_Rotation += rotation;
	}

	void Object::AddScale(const glm::vec3& scale)
	{
		m_Scale += scale;
	}

	glm::mat4 Object::GetTransformationMatrix()
	{
		return glm::translate(glm::mat4(1.0f), m_Translation) * glm::yawPitchRoll(glm::radians(m_Rotation.x), glm::radians(m_Rotation.y), glm::radians(m_Rotation.z)) * glm::scale(glm::mat4(1.0f), m_Scale);
	}

	void Object::LookAtPosition(const glm::vec3& position)
	{
		glm::vec3 newDirection = glm::normalize(m_Translation - position);

		m_Rotation.x = glm::degrees(-atan2(newDirection.z, newDirection.x)) + 90.0f;
		m_Rotation.y = glm::degrees(-glm::asin(newDirection.y));
	}

	void Object::LookInDirection(const glm::vec3& forwardDirection)
	{
		m_Rotation.x = glm::degrees(-atan2(forwardDirection.z, forwardDirection.x)) + 90.0f;
		m_Rotation.y = glm::degrees(-glm::asin(forwardDirection.y));
	}

	void Object::LookInDirectionX(const glm::vec3& forwardDirection)
	{
		m_Rotation.x = glm::degrees(-atan2(forwardDirection.z, forwardDirection.x)) + 90.0f;
		//m_Rotation.y = glm::degrees(-glm::asin(forwardDirection.y));
	}

	void Object::GenerateFinalTransformation(const glm::mat4& offset)
	{
		m_FinalTransformation = offset * GetTransformationMatrix();
	}

	void Object::SetFinalTransformation(const glm::mat4& transformation)
	{
		m_FinalTransformation = transformation;
	}

	void Object::Draw(const glm::mat4& projection, const glm::mat4& view)
	{
		if (m_Mesh && m_Texture && m_Shader) {
			m_Shader->Bind();
			m_Texture->Bind();
			m_Mesh->Bind();

			m_Shader->SetMat4("M", m_FinalTransformation);
			m_Shader->SetMat4("V", view);
			m_Shader->SetMat4("P", projection);

			m_Mesh->Draw();
		}
	}

	void Object::Draw(const glm::mat4& projection, const glm::mat4& view, const glm::mat4& modelOffset)
	{
		if (m_Mesh && m_Texture && m_Shader) {
			m_Shader->Bind();
			m_Texture->Bind();
			m_Mesh->Bind();

			m_FinalTransformation = modelOffset * GetTransformationMatrix();

			m_Shader->SetMat4("M", m_FinalTransformation);
			m_Shader->SetMat4("V", view);
			m_Shader->SetMat4("P", projection);

			m_Mesh->Draw();
		}
	}

	Object* Object::DeepCopy(Object* object)
	{
		Object* copy = new Object(object->m_Mesh->GetPath(), object->m_Shader->GetPath(), object->m_Texture->GetPath(), object->m_IsDynamic);
		copy->m_Translation = object->m_Translation;
		copy->m_Rotation = object->m_Rotation;
		copy->m_Scale = object->m_Scale;
		return copy;
	}
}