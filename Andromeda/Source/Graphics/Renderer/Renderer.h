#pragma once

#include "Object.h"

#include "glm/glm.hpp"

#include <deque>

namespace AD {
	class Renderer {
	protected:
		std::deque<Object*> m_Queue;

	public:
		Renderer();
		~Renderer();

		void AddObject(Object* object);

		virtual void Flush(const glm::mat4& projection, const glm::mat4& view) = 0;

		static void InitializeSingleton(Renderer* renderer);
		static Renderer* GetInstance();
		static void CleanupSingleton();
	};
}