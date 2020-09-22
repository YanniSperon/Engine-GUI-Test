#include "Node.h"

#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

namespace AD {

	static int s_ID = 0;

	Node::Node()
		: m_Camera(nullptr), m_Object(nullptr), m_Light(nullptr), m_Children(), m_Parent(nullptr), m_Translation(0.0f), m_Rotation(0.0f), m_Name("Node"), m_ID(s_ID)
	{
		s_ID++;
	}

	Node::Node(Node* parent)
		: m_Camera(nullptr), m_Object(nullptr), m_Light(nullptr), m_Children(), m_Parent(parent), m_Translation(0.0f), m_Rotation(0.0f), m_Name("Node"), m_ID(s_ID)
	{
		m_Parent->AddChild(this);
		s_ID++;
	}

	Node::~Node()
	{
		for (int i = 0; i < m_Children.size(); i++) {
			delete m_Children[i];
		}
	}

	void Node::Update(float deltaTime)
	{
		
	}

	void Node::Draw(const glm::mat4& projection, const glm::mat4& view, const glm::mat4& currentTransformationMatrix)
	{
		glm::mat4 newTransformationMatrix = currentTransformationMatrix * GetTransformationMatrix();
		for (int i = 0; i < m_Children.size(); i++) {
			m_Children[i]->Draw(projection, view, newTransformationMatrix);
		}
		if (m_Object) {
			m_Object->Draw(projection, view, newTransformationMatrix * m_Object->GetTransformationMatrix());
		}
	}

	void Node::AddChild(Node* child)
	{
		m_Children.push_back(child);
		child->m_Parent = this;
	}

	Node* Node::SearchParentAndChildrenForNode(int id)
	{
		Node* rootNode = this;
		Node* tempParent = rootNode->m_Parent;
		while (tempParent) {
			rootNode = tempParent;
			tempParent = rootNode->m_Parent;
		}

		return rootNode->SearchChildrenForNode(id);
	}

	Node* Node::SearchChildrenForNode(int id)
	{
		Node* returnValue = nullptr;
		if (this->m_ID == id) {
			return this;
		}
		for (int i = 0; i < m_Children.size(); i++) {
			Node* temp = m_Children.at(i)->SearchChildrenForNode(id);
			if (temp) {
				returnValue = temp;
			}
		}
		return returnValue;
	}

	glm::mat4 Node::GetTransformationMatrix()
	{
		return glm::translate(glm::mat4(), m_Translation) * glm::yawPitchRoll(glm::radians(m_Rotation.x), glm::radians(m_Rotation.y), glm::radians(m_Rotation.z));
	}

	Node* Node::GetParent()
	{
		return m_Parent;
	}

	Node* Node::DeepCopy(Node* node)
	{
		return DeepCopyRecursive(node, true);
	}

	Node* Node::ShallowCopy(Node* node)
	{
		if (node->m_Parent) {
			Node* copy = new Node(node->m_Parent);
			if (node->m_Object) {
				copy->m_Object = Object::DeepCopy(node->m_Object);
			}
			if (node->m_Camera) {
				copy->m_Camera = Camera::DeepCopy(node->m_Camera);
			}
			if (node->m_Light) {
				copy->m_Light = Light::DeepCopy(node->m_Light);
			}
			copy->m_Name = "Node";
			copy->m_Translation = node->m_Translation;
			copy->m_Rotation = node->m_Rotation;

			return copy;
		}
		return nullptr;
	}

	Node* Node::DeepCopyRecursive(Node* node, bool isFirst)
	{
		if (node->m_Parent) {
			Node* copyRoot = nullptr;

			if (!isFirst) {
				copyRoot = new Node();
				if (node->m_Object) {
					copyRoot->m_Object = Object::DeepCopy(node->m_Object);
				}
				if (node->m_Camera) {
					copyRoot->m_Camera = Camera::DeepCopy(node->m_Camera);
				}
				if (node->m_Light) {
					copyRoot->m_Light = Light::DeepCopy(node->m_Light);
				}
				copyRoot->m_Name = "Node";
				copyRoot->m_Translation = node->m_Translation;
				copyRoot->m_Rotation = node->m_Rotation;
			}
			else {
				copyRoot = Node::ShallowCopy(node);
			}

			for (int i = 0; i < node->m_Children.size(); i++) {
				copyRoot->AddChild(Node::DeepCopyRecursive(node->m_Children[i], false));
			}

			return copyRoot;
		}
		return nullptr;
	}
}