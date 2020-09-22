#pragma once

#include "Layer.h"

namespace AD {
	class Layer2D : public Layer {
	private:

	public:
		Layer2D(int width, int height, int x, int y);
		~Layer2D();

		void Update(float deltaTime) override;
		void Draw() override;
	};
}
