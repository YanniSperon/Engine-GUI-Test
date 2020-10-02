#pragma once

#include <imgui.h>

namespace AD {
	class GUI {
	private:
		ImGuiIO* m_IO;
		ImFont* m_Font;
	public:
		GUI();
		~GUI();

		bool WantCaptureMouse();
		bool WantCaptureKeyboard();

		void PressButton(int button);
		void ReleaseButton(int button);
		void MoveMouse(float x, float y);
		void ScrollMouseWheel(float x, float y);
		void PressKey(int key);
		void ReleaseKey(int key);
		void TypeKey(int key);
		void Resize(int x, int y);
		
		// Call at the top of the window update function
		void UpdateWindow(float deltaTime);
		// Call before each 2D layers drawing
		void Prepare();
		// Call after each 2D layers drawing
		void Flush();

		ImGuiIO* GetIO();
		ImFont* GetFont();

		static void InitializeSingleton();
		static GUI* GetInstance();
		static void CleanupSingleton();
	};
}