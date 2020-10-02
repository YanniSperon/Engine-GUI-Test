#pragma once

#include "Layer.h"
#include "Scene.h"

#include <memory>

namespace AD {
	class Layer3D : public Layer {
	protected:
		std::unique_ptr<Scene> m_Scene;
	public:
		Layer3D(int width, int height, int x, int y);
		virtual ~Layer3D() override;

		virtual void Update(float deltaTime) override;
		virtual void Draw() override;

		Scene& GetScene();
	};
}
