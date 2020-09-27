#pragma once

#include "Renderer.h"

namespace AD {
	class LinearRenderer : public Renderer {
	private:

	public:
		LinearRenderer();

		void Flush(const glm::mat4& projection, const glm::mat4& view) override;
	};
}