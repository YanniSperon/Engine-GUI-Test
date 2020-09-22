#pragma once

#include "Layer.h"

#include <functional>

namespace AD {
	class LayerCustom : public Layer {
	private:
		std::function<void(float)> m_CustomUpdateCode;
		std::function<void()> m_CustomDrawCode;
	public:
		LayerCustom(LayerType type, const std::function<void(float)>& customUpdateCode, const std::function<void()>& customDrawCode, int width, int height, int x, int y);
		~LayerCustom();

		void Update(float deltaTime) override;
		void Draw() override;
	};
}