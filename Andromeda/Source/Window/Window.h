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
#include <functional>

namespace AD {

	class Window {
	private:
		std::vector<Layer*> m_Layers;
		GLFWwindow* m_Window;
		int m_Width;
		int m_Height;
		int m_CurrentLayer;
		bool m_HasVSync;
		bool m_HasRawMouseMotion;
		bool m_DidMouseMove;
		Input m_Input;

		void SetWidth(int x);
		void SetHeight(int y);
	public:
		Window(const std::string& name, int width, int height);
		~Window();

		void SetIcon(const std::string& path);
		
		void Prepare();
		void DispatchMouseInput(uint16_t button, uint16_t event);
		void DispatchKeyboardInput(uint16_t button, uint16_t event);
		void DispatchMouseMovement(double x, double y);
		void DispatchMouseScroll(double x, double y);
		void DispatchKeyTyped(unsigned int keycode);
		void DispatchCursorEnterLeave(int didEnter);
		void DispatchWindowResize(int width, int height);

		void Update(float deltaTime);
		void Draw();
		void Flush();

		Layer2D* AddLayer2D(Layer2DType layer2DType);
		Layer3D* AddLayer3D();
		LayerCustom* AddLayerCustom(const std::function<void(float)>& customUpdateCode, const std::function<void()>& customDrawCode);

		int GetCurrentLayerIndex();
		int GetWidth();
		int GetHeight();
		bool GetHasVSync();
		bool GetDidMouseMove();

		void SetVSync(bool vsync);
		void SetCurrentLayerIndex(int index);
		void IncrementCurrentLayer();

		bool EnableRawMouseMotion();
		bool ShouldCloseWindow();
		GLFWwindow* GetGLFWwindow();

		Layer* GetLayer(int index);
		Layer* GetCurrentLayer();
		Layer* GetLastLayer();

		void RemoveLayer(int index);
		void RemoveLayer(Layer* layer);

		Input& GetInput();

		void HideMouse();
		void ShowMouse();

		static void InitializeSingleton();
		static Window* GetInstance();
		static void CleanupSingleton();
	};
}
