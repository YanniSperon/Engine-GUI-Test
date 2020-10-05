#pragma once

#include "Object.h"
#include "Camera.h"
#include "Light.h"
#include "Renderer.h"

#include <glm/glm.hpp>

#include <vector>
#include <string>

namespace AD {
	class Node {
	private:
		Object* m_Object;
		Camera* m_Camera;
		Light* m_Light;

		std::vector<Node*> m_Children;
		Node* m_Parent;

		std::string m_Name;

		glm::vec3 m_Translation;
		glm::vec3 m_Rotation;
		int m_ID;
	public:
		// Construct a root node
		Node();
		// Construct a child node
		Node(Node* parent);

		~Node();

		void Update(float deltaTime);
		void Draw(Renderer* renderer);

		void SetObjectComponent(Object* object);
		void SetCameraComponent(Camera* camera);
		void SetLightComponent(Light* light);
		void SetName(const std::string& name);

		void AddChild(Node* child);
		void CreateChild();
		void MoveNode(Node* newParent);
		Node* RemoveChild(Node* child);
		Node* RemoveChild(int id);
		void DeleteChild(Node* child);
		void DeleteChild(int id);
		void RemoveFromParent();

		Node* SearchParentAndChildrenForNode(int id);
		Node* SearchChildrenForNode(int id);

		glm::mat4 GetTransformationMatrix();
		Node* GetParent();
		std::string GetName();
		Object* GetObjectComponent();
		Camera* GetCameraComponent();
		Light* GetLightComponent();
		int GetID();
		std::vector<Node*>& GetChildren();
		int GetNumNodes();

		bool GetHasObjectComponent();
		bool GetHasCameraComponent();
		bool GetHasLightComponent();

		void AddObjectComponent(Object* object);
		void AddCameraComponent(Camera* camera);
		void AddLightComponent(Light* light);

		void RemoveObjectComponent();
		void RemoveCameraComponent();
		void RemoveLightComponent();

		glm::vec3& GetTranslationRef();
		glm::vec3& GetRotationRef();
		std::string& GetNameRef();

		const glm::vec3& GetTranslation();
		const glm::vec3& GetRotation();

		void SetTranslation(const glm::vec3& translation);
		void SetRotation(const glm::vec3& rotation);

		static Node* DeepCopyTo(Node* node, Node* resultingParent);
		// Copies children nodes
		static Node* DeepCopy(Node* node);
		// Doesn't copy children nodes
		static Node* ShallowCopy(Node* node);
	private:
		static Node* DeepCopyRecursive(Node* node, bool isFirst);
		void UpdatePositioningOfChildrenRecursive(float deltaTime, const glm::mat4& currentTransformationMatrix);
	};
}