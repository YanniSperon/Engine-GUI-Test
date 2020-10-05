#pragma once

#include <string>
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
	public:
		static void PlaceTextLeftX(const char* fmt, ...);
		static void PlaceTextCenterX(const char* fmt, ...);
		static void PlaceTextRightX(const char* fmt, ...);

		static void PlaceTextCenter(const char* fmt, ...);
		static void SameLine();

		// Percentage of the screen from the top left
		// Percent is between 0-1
		static void PlaceTextAtPercentage(float percentageX, float percentageY, const char* fmt, ...);
		static void MoveCursorToPercentage(float percentageX, float percentageY, float sizeX, float sizeY);

		// Position is the position of the top left of the window relative to the min and max
		// Position and size are between 0 and 1
		// Min and Max are pixel values
		static void PlaceWindow(float positionX, float positionY, float sizeX, float sizeY, float minX, float minY, float maxX, float maxY);
		// Position is the position of the center of the window relative to the min and max
		// Position and size are between 0 and 1
		// Min and Max are pixel values
		static void PlaceWindowCenter(float positionX, float positionY, float sizeX, float sizeY, float minX, float minY, float maxX, float maxY);

		static void SetDarkTheme();
		static void SetLightTheme();
		static void SetEmptyTheme();

		static float GetTextSizeX(const std::string& text);
		static float GetTextSizeY(const std::string& text);
	};
}