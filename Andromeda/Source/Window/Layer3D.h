#pragma once

#include "Layer.h"

#include <memory>

namespace AD {
	class Layer3D : public Layer {
	private:
		//std::unique_ptr<Scene> m_Scene;
	public:
		Layer3D(int width, int height, int x, int y);
		~Layer3D();

		void Update(float deltaTime) override;
		void Draw() override;
	};
}
