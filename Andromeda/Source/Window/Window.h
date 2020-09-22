#pragma once

#include "Layer.h"
#include "Layer2D.h"
#include "Layer3D.h"
#include "LayerCustom.h"

#include <glew.h>
#include <glfw3.h>

#include <vector>
#include <cstdint>
#include <string>
#include <memory>
#include <functional>

namespace AD {

	class Window {
	private:
		std::vector<std::unique_ptr<Layer>> m_Layers;
		GLFWwindow* m_Window;
		int m_Width;
		int m_Height;
		int m_CurrentLayer;
		bool m_HasVSync;
		bool m_HasRawMouseMotion;

		double m_CursorXPos;
		double m_CursorYPos;
		bool m_MouseMoved;

		void SetCursorX(double x);
		void SetCursorY(double y);
		void SetWidth(int x);
		void SetHeight(int y);
		void SetMouseMoved(bool didMove);
	public:
		Window(const std::string& name, int width, int height);
		~Window();
		
		void Prepare();
		void DispatchMouseInput(uint16_t button, uint16_t event);
		void DispatchKeyboardInput(uint16_t button, uint16_t event);
		void Update(float deltaTime);
		void Draw();
		void Flush();

		void AddLayer2D();
		void AddLayer3D();
		void AddLayerCustom(LayerType type, const std::function<void(float)>& customUpdateCode, const std::function<void()>& customDrawCode);

		int GetCurrentLayerIndex();
		int GetWidth();
		int GetHeight();
		bool GetHasVSync();
		double GetCursorX();
		double GetCursorY();
		bool GetMouseMoved();

		void SetVSync(bool vsync);
		void SetCurrentLayerIndex(int index);
		void IncrementCurrentLayer();

		bool EnableRawMouseMotion();
		bool ShouldCloseWindow();
		GLFWwindow* GetGLFWwindow();

		Layer& GetLayer(int index);
		Layer& GetCurrentLayer();
		Layer& GetLastLayer();

		static void InitializeSingleton();
		static Window* GetInstance();
		static void CleanupSingleton();
	};
}
