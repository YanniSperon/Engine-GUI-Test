#include "LinearRenderer.h"
#include "Console.h"

namespace AD {
	LinearRenderer::LinearRenderer()
		: Renderer()
	{

	}

	void LinearRenderer::Flush(const glm::mat4& projection, const glm::mat4& view)
	{
		while (!m_Queue.empty()) {
			Object* renderable = m_Queue.front();
			renderable->Draw(projection, view);
			m_Queue.pop_front();
		}
	}
}