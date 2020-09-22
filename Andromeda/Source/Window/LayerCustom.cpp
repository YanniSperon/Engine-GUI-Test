#include "LayerCustom.h"

namespace AD {
	LayerCustom::LayerCustom(LayerType type, const std::function<void(float)>& customUpdateCode, const std::function<void()>& customDrawCode, int width, int height, int x, int y)
		: Layer(type, width, height, x, y), m_CustomUpdateCode(customUpdateCode), m_CustomDrawCode(customDrawCode)
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
		m_CustomDrawCode();
	}
}