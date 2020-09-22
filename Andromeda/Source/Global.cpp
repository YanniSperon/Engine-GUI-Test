#include "Global.h"
#include "Console.h"

#include "Config.h"
#include "Mesh3DManager.h"
#include "TextureManager.h"
#include "ShaderManager.h"
#include "Window.h"

#include <memory>

namespace AD
{
	static std::vector<std::string> g_ConsoleLog;
	static std::chrono::steady_clock::time_point g_SystemStartTime;

	void AD::Global::Initialize()
	{
		g_ConsoleLog = std::vector<std::string>();
		g_SystemStartTime = std::chrono::high_resolution_clock::now();

		Config::InitializeSingleton();
		Mesh3DManager::InitializeSingleton();
		TextureManager::InitializeSingleton();
		ShaderManager::InitializeSingleton();
		Window::InitializeSingleton();
	}

	std::chrono::steady_clock::time_point& AD::Global::GetStartTime()
	{
		return g_SystemStartTime;
	}

	std::vector<std::string>& AD::Global::GetConsoleLog()
	{
		return g_ConsoleLog;
	}

	void Global::Update()
	{
		Mesh3DManager::GetInstance()->Cleanup();
		TextureManager::GetInstance()->Cleanup();
		ShaderManager::GetInstance()->Cleanup();
	}

	void Global::Cleanup()
	{
		Config::CleanupSingleton();
		Mesh3DManager::CleanupSingleton();
		TextureManager::CleanupSingleton();
		ShaderManager::CleanupSingleton();
		Window::CleanupSingleton();
	}
}
