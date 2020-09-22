#pragma once

#include "Object.h"
#include "Camera.h"
#include "Light.h"

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
		void Draw(const glm::mat4& projection, const glm::mat4& view, const glm::mat4& currentTransformationMatrix);

		void AddChild(Node* child);

		Node* SearchParentAndChildrenForNode(int id);
		Node* SearchChildrenForNode(int id);

		glm::mat4 GetTransformationMatrix();
		Node* GetParent();

		// Copies children nodes
		static Node* DeepCopy(Node* node);
		// Doesn't copy children nodes
		static Node* ShallowCopy(Node* node);
	private:
		static Node* DeepCopyRecursive(Node* node, bool isFirst);
	};
}