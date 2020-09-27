#include "Renderer.h"

namespace AD {
	Renderer* s_Renderer;

	Renderer::Renderer()
		: m_Queue()
	{

	}

	Renderer::~Renderer()
	{

	}

	void Renderer::AddObject(Object* object)
	{
		m_Queue.push_back(object);
	}

	void Renderer::InitializeSingleton(Renderer* renderer)
	{
		s_Renderer = renderer;
	}

	Renderer* Renderer::GetInstance()
	{
		return s_Renderer;
	}

	void Renderer::CleanupSingleton()
	{
		delete s_Renderer;
	}
}