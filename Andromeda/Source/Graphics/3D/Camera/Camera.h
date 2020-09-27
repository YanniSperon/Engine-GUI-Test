#pragma once

#include <glm/glm.hpp>

#include <string>

namespace AD {
	class Camera {
	private:
		glm::vec3 m_Rotation;
		glm::vec3 m_Translation;

		float m_NearPlane;
		float m_FarPlane;

		float m_MovementSpeed;

		float m_OldMouseX;
		float m_OldMouseY;

		bool m_WasUsedYet;
		bool m_IsFocused;
	public:
		Camera();
		~Camera();

		void Update(float deltaTime);

		void SetNearPlane(float nearPlane);
		void SetFarPlane(float farPlane);
		void SetMovementSpeed(float movementSpeed);
		void SetOldMouseX(float x);
		void SetOldMouseY(float y);
		void SetTranslation(const glm::vec3& translation);
		void SetUpDirection(const glm::vec3& upDirection);
		void SetWasUsedYet(bool wasUsedYet);
		void SetIsFocused(bool isFocused);

		const float GetNearPlane();
		const float GetFarPlane();
		const float GetMovementSpeed();
		const float GetOldMouseX();
		const float GetOldMouseY();
		const glm::vec3& GetTranslation();
		const glm::vec3& GetRotation();
		const bool GetWasUsedYet();
		glm::vec3 GetUpDirection();
		glm::vec3 GetForwardDirection();
		glm::mat4 GetProjectionMatrix();
		glm::mat4 GetViewMatrix();
		const bool GetIsFocused();

		void LookAtPosition(const glm::vec3& position);
		void LookInDirection(const glm::vec3& forwardDirection);
		void LookAtMouse(float sensitivity, float xPos, float yPos);

		void MoveForward(float deltaTime);
		void MoveBackward(float deltaTime);
		void StrafeLeft(float deltaTime);
		void StrafeRight(float deltaTime);
		void MoveUp(float deltaTime);
		void MoveDown(float deltaTime);

		void SetRotation(const glm::vec3& rotation);

		void AddTranslation(const glm::vec3& translation);
		void AddRotation(const glm::vec3& rotation);

		std::string ToString();

		static Camera* DeepCopy(Camera* camera);
	};
}