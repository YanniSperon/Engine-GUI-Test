#include "Scene.h"
#include "Console.h"

namespace AD {

	Scene::Scene()
		: m_RootNode(new Node()), m_ActiveCameraNode(m_RootNode)
	{
		m_RootNode->SetName("Root");
		m_RootNode->AddCameraComponent(new Camera());
	}

	Scene::~Scene()
	{
		delete m_RootNode;
	}

	void Scene::Update(float deltaTime)
	{
		m_ActiveCameraNode->GetCameraComponent()->SetIsFocused(true);
		m_RootNode->Update(deltaTime);
		m_ActiveCameraNode->GetCameraComponent()->SetIsFocused(false);
	}

	void Scene::Draw()
	{
		m_RootNode->Draw(Renderer::GetInstance());
		Renderer::GetInstance()->Flush(m_ActiveCameraNode->GetCameraComponent()->GetProjectionMatrix(), m_ActiveCameraNode->GetCameraComponent()->GetViewMatrix());
	}

	Node* Scene::GetRootNode()
	{
		return m_RootNode;
	}

	void Scene::AddNode(Node* object)
	{
		m_RootNode->AddChild(object);
	}

	Node* Scene::GetNodeByID(int id)
	{
		return m_RootNode->SearchChildrenForNode(id);
	}

	void Scene::SetActiveCamera(Node* newCameraNode)
	{
		m_ActiveCameraNode = newCameraNode;
	}
}