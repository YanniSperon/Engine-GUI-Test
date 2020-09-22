#include "Layer3D.h"

#include <glew.h>

namespace AD {
	Layer3D::Layer3D(int width, int height, int x, int y)
		: Layer(LayerType::THREED, width, height, x, y)//, m_Scene()
	{

	}

	Layer3D::~Layer3D()
	{

	}

	void Layer3D::Update(float deltaTime)
	{
		//m_Scene.Update(deltaTime)
	}

	void Layer3D::Draw()
	{
		glViewport(m_XPos, m_YPos, m_Width, m_Height);
		//m_Scene.Draw();
	}
}
