#include "Layer2D.h"

#include <glew.h>

namespace AD {

	Layer2D::Layer2D(int width, int height, int x, int y)
		: Layer(LayerType::TWOD, width, height, x, y)
	{

	}

	Layer2D::~Layer2D()
	{
	}

	void Layer2D::Update(float deltaTime)
	{
	}

	void Layer2D::Draw()
	{
		glViewport(m_XPos, m_YPos, m_Width, m_Height);
	}
}
