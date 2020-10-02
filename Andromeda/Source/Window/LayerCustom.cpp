#include "LayerCustom.h"

#include <glew.h>

namespace AD {
	LayerCustom::LayerCustom(const std::function<void(float)>& customUpdateCode, const std::function<void()>& customDrawCode, int width, int height, int x, int y)
		: Layer(LayerType::CUSTOM, width, height, x, y, "Custom Layer"), m_CustomUpdateCode(customUpdateCode), m_CustomDrawCode(customDrawCode)
	{

	}

	LayerCustom::~LayerCustom()
	{

	}

	void LayerCustom::Update(float deltaTime)
	{
		m_CustomUpdateCode(deltaTime);
	}

	void LayerCustom::Draw()
	{
		glViewport(m_XPos, m_YPos, m_Width, m_Height);
		m_CustomDrawCode();
	}
}