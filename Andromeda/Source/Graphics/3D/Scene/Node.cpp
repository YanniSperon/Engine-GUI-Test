#include "Node.h"
#include "Console.h"

#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

namespace AD {

	static int s_ID = 0;

	Node::Node()
		: m_Camera(nullptr), m_Object(nullptr), m_Light(nullptr), m_Children(), m_Parent(nullptr), m_Translation(0.0f), m_Rotation(0.0f), m_Name("Node"), m_ID(s_ID)
	{
		s_ID++;
		m_Name.resize(64);
	}

	Node::Node(Node* parent)
		: m_Camera(nullptr), m_Object(nullptr), m_Light(nullptr), m_Children(), m_Parent(parent), m_Translation(0.0f), m_Rotation(0.0f), m_Name("Node"), m_ID(s_ID)
	{
		m_Parent->AddChild(this);
		s_ID++;
		m_Name.resize(64);
	}

	Node::~Node()
	{
		delete m_Object;
		delete m_Camera;
		delete m_Light;
		for (int i = 0; i < m_Children.size(); i++) {
			delete m_Children[i];
		}
	}

	void Node::Update(float deltaTime)
	{
		UpdatePositioningOfChildrenRecursive(deltaTime, glm::mat4(1.0f));
	}

	void Node::UpdatePositioningOfChildrenRecursive(float deltaTime, const glm::mat4& currentTransformationMatrix)
	{
		glm::mat4 newTransformationMatrix = currentTransformationMatrix * GetTransformationMatrix();
		if (m_Object) {
			m_Object->GenerateFinalTransformation(newTransformationMatrix);
		}
		if (m_Camera) {
			m_Camera->Update(deltaTime);
		}
		for (int i = 0; i < m_Children.size(); i++) {
			m_Children[i]->UpdatePositioningOfChildrenRecursive(deltaTime, newTransformationMatrix);
		}
	}

	void Node::Draw(Renderer* renderer)
	{
		if (m_Object) {
			renderer->AddObject(m_Object);
		}
		for (int i = 0; i < m_Children.size(); i++) {
			m_Children[i]->Draw(renderer);
		}
	}

	void Node::SetObjectComponent(Object* object)
	{
		if (m_Object) {
			Object* temp = m_Object;
			m_Object = object;
			delete temp;
		}
		else {
			m_Object = object;
		}
	}

	void Node::SetCameraComponent(Camera* camera)
	{
		if (m_Camera) {
			Camera* temp = m_Camera;
			m_Camera = camera;
			delete temp;
		}
		else {
			m_Camera = camera;
		}
	}

	void Node::SetLightComponent(Light* light)
	{
		if (m_Light) {
			Light* temp = m_Light;
			m_Light = light;
			delete temp;
		}
		else {
			m_Light = light;
		}
	}

	void Node::SetName(const std::string& name)
	{
		m_Name = name;
		m_Name.resize(64);
	}

	void Node::AddChild(Node* child)
	{
		m_Children.push_back(child);
		child->m_Parent = this;
	}

	void Node::CreateChild()
	{
		new Node(this);
	}

	void Node::MoveNode(Node* newParent)
	{
		m_Parent->RemoveChild(this);
		m_Parent->AddChild(this);
	}

	Node* Node::RemoveChild(Node* child)
	{
		for (int i = 0; i < m_Children.size(); i++) {
			if (m_Children[i] == child) {
				Node* temp = m_Children[i];
				m_Children.erase(m_Children.begin() + i);
				return temp;
			}
		}
		return nullptr;
	}

	Node* Node::RemoveChild(int id)
	{
		for (int i = 0; i < m_Children.size(); i++) {
			if (m_Children[i]->GetID() == id) {
				Node* temp = m_Children[i];
				m_Children.erase(m_Children.begin() + i);
				return temp;
			}
		}
		return nullptr;
	}

	void Node::DeleteChild(Node* child)
	{
		for (int i = 0; i < m_Children.size(); i++) {
			if (m_Children[i] == child) {
				delete m_Children[i];
				m_Children.erase(m_Children.begin() + i);
			}
		}
	}

	void Node::DeleteChild(int id)
	{
		for (int i = 0; i < m_Children.size(); i++) {
			if (m_Children[i]->GetID() == id) {
				delete m_Children[i];
				m_Children.erase(m_Children.begin() + i);
			}
		}
	}

	void Node::RemoveFromParent()
	{
		m_Parent->RemoveChild(this);
		m_Parent = nullptr;
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
		return glm::translate(glm::mat4(1.0f), m_Translation) * glm::yawPitchRoll(glm::radians(m_Rotation.x), glm::radians(m_Rotation.y), glm::radians(m_Rotation.z));
	}

	Node* Node::GetParent()
	{
		return m_Parent;
	}

	std::string Node::GetName()
	{
		return m_Name;
	}

	Object* Node::GetObjectComponent()
	{
		return m_Object;
	}

	Camera* Node::GetCameraComponent()
	{
		return m_Camera;
	}

	Light* Node::GetLightComponent()
	{
		return m_Light;
	}

	int Node::GetID()
	{
		return m_ID;
	}

	std::vector<Node*>& Node::GetChildren()
	{
		return m_Children;
	}

	int Node::GetNumNodes()
	{
		int count = 1;
		for (int i = 0; i < m_Children.size(); i++) {
			count += m_Children[i]->GetNumNodes();
		}
		return count;
	}

	bool Node::GetHasObjectComponent()
	{
		return m_Object;
	}

	bool Node::GetHasCameraComponent()
	{
		return m_Camera;
	}

	bool Node::GetHasLightComponent()
	{
		return m_Light;
	}

	void Node::AddObjectComponent(Object* object)
	{
		m_Object = object;
	}

	void Node::AddCameraComponent(Camera* camera)
	{
		m_Camera = camera;
	}

	void Node::AddLightComponent(Light* light)
	{
		m_Light = light;
	}

	void Node::RemoveObjectComponent()
	{
		delete m_Object;
		m_Object = nullptr;
	}

	void Node::RemoveCameraComponent()
	{
		delete m_Camera;
		m_Camera = nullptr;
	}

	void Node::RemoveLightComponent()
	{
		delete m_Light;
		m_Light = nullptr;
	}

	glm::vec3& Node::GetTranslationRef()
	{
		return m_Translation;
	}

	glm::vec3& Node::GetRotationRef()
	{
		return m_Rotation;
	}

	std::string& Node::GetNameRef()
	{
		return m_Name;
	}

	const glm::vec3& Node::GetTranslation()
	{
		return m_Translation;
	}

	const glm::vec3& Node::GetRotation()
	{
		return m_Rotation;
	}

	void Node::SetTranslation(const glm::vec3& translation)
	{
		m_Translation = translation;
	}

	void Node::SetRotation(const glm::vec3& rotation)
	{
		m_Rotation = rotation;
	}

	Node* Node::DeepCopyTo(Node* node, Node* resultingParent)
	{
		Node* temp = DeepCopyRecursive(node, true);
		temp->RemoveFromParent();
		resultingParent->AddChild(temp);
		return temp;
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
			copy->m_Name.resize(64);
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
				copyRoot->m_Name.resize(64);
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