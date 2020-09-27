#include "Layer3D.h"
#include "Console.h"

#include <glew.h>

namespace AD {
	Layer3D::Layer3D(int width, int height, int x, int y)
		: Layer(LayerType::THREED, width, height, x, y), m_Scene(std::make_unique<Scene>())
	{
		Node* child1 = new Node();
		child1->AddObjectComponent(new Object("Resources/Cube.obj", "Resources/Basic", "Resources/Basic.png", false));
		child1->GetObjectComponent()->SetTranslation(glm::vec3(0.0f, 0.0f, 5.0f));
		m_Scene->AddNode(child1);
	}

	Layer3D::~Layer3D()
	{

	}

	void Layer3D::Update(float deltaTime)
	{
		m_Scene->Update(deltaTime);
	}

	void Layer3D::Draw()
	{
		glViewport(m_XPos, m_YPos, m_Width, m_Height);
		m_Scene->Draw();
	}
}
