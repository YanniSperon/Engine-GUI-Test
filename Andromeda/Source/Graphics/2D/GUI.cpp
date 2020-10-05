#include "GUI.h"
#include "Input.h"
#include "Window.h"
#include "Console.h"

#include <ImGuiOpenGLRenderer.h>

//#define ARIAL_FONT
//#define TIMES_NEW_ROMAN_FONT
#define TAHOMA_FONT

#define GUI_INDENTATION_SIZE 5.0f
#define GUI_FONT_SIZE 20.0f

namespace AD {

	static GUI* s_GUI;

	GUI::GUI()
		: m_IO(nullptr), m_Font(nullptr)
	{
		ImGui::CreateContext();
		ImGui::StyleColorsDark();

		m_IO = &ImGui::GetIO();

		m_IO->DisplaySize = ImVec2(Window::GetInstance()->GetWidth(), Window::GetInstance()->GetHeight());

#if defined(ARIAL_FONT)
		static auto m_Font = m_IO->Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\arial.ttf", GUI_FONT_SIZE);
#elif defined(TAHOMA_FONT)
		static auto m_Font = m_IO->Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\tahoma.ttf", GUI_FONT_SIZE);
#elif defined(TIMES_NEW_ROMAN_FONT)
		static auto m_Font = m_IO->Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\times.ttf", GUI_FONT_SIZE);
#endif

		m_IO->IniFilename = nullptr;
		m_IO->BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		m_IO->BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

		// Temporary, should convert to AD key codes
		m_IO->KeyMap[ImGuiKey_Tab] = AD_KEY_TAB;
		m_IO->KeyMap[ImGuiKey_LeftArrow] = AD_KEY_LEFT;
		m_IO->KeyMap[ImGuiKey_RightArrow] = AD_KEY_RIGHT;
		m_IO->KeyMap[ImGuiKey_UpArrow] = AD_KEY_UP;
		m_IO->KeyMap[ImGuiKey_DownArrow] = AD_KEY_DOWN;
		m_IO->KeyMap[ImGuiKey_PageUp] = AD_KEY_PAGE_UP;
		m_IO->KeyMap[ImGuiKey_PageDown] = AD_KEY_PAGE_DOWN;
		m_IO->KeyMap[ImGuiKey_Home] = AD_KEY_HOME;
		m_IO->KeyMap[ImGuiKey_End] = AD_KEY_END;
		m_IO->KeyMap[ImGuiKey_Insert] = AD_KEY_INSERT;
		m_IO->KeyMap[ImGuiKey_Delete] = AD_KEY_DELETE;
		m_IO->KeyMap[ImGuiKey_Backspace] = AD_KEY_BACKSPACE;
		m_IO->KeyMap[ImGuiKey_Space] = AD_KEY_SPACE;
		m_IO->KeyMap[ImGuiKey_Enter] = AD_KEY_ENTER;
		m_IO->KeyMap[ImGuiKey_Escape] = AD_KEY_ESCAPE;
		m_IO->KeyMap[ImGuiKey_KeyPadEnter] = AD_KEY_KP_ENTER;
		m_IO->KeyMap[ImGuiKey_A] = AD_KEY_A;
		m_IO->KeyMap[ImGuiKey_C] = AD_KEY_C;
		m_IO->KeyMap[ImGuiKey_V] = AD_KEY_V;
		m_IO->KeyMap[ImGuiKey_X] = AD_KEY_X;
		m_IO->KeyMap[ImGuiKey_Y] = AD_KEY_Y;
		m_IO->KeyMap[ImGuiKey_Z] = AD_KEY_Z;

		ImGui_ImplOpenGL3_Init("#version 460");
	}

	GUI::~GUI()
	{
		ImGui_ImplOpenGL3_Shutdown();
	}

	bool GUI::WantCaptureMouse()
	{
		return m_IO->WantCaptureMouse;
	}

	bool GUI::WantCaptureKeyboard()
	{
		return m_IO->WantCaptureKeyboard;
	}

	void GUI::Resize(int width, int height)
	{
		m_IO->DisplaySize = ImVec2(width, height);
		m_IO->DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
	}

	void GUI::UpdateWindow(float deltaTime)
	{
		m_IO->DeltaTime = deltaTime;
	}

	void GUI::Prepare()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();
		ImGui::PushFont(m_Font);
	}

	void GUI::Flush()
	{
		ImGui::PopFont();
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	ImGuiIO* GUI::GetIO()
	{
		return m_IO;
	}

	ImFont* GUI::GetFont()
	{
		return m_Font;
	}

	void GUI::InitializeSingleton()
	{
		s_GUI = new GUI();
	}

	GUI* GUI::GetInstance()
	{
		return s_GUI;
	}

	void GUI::CleanupSingleton()
	{
		delete s_GUI;
	}

	void GUI::PlaceTextLeftX(const char* fmt, ...)
	{
		va_list args;
		va_start(args, fmt);
		char* txt = new char[1024];
		vsnprintf(txt, 1024, fmt, args);
		std::string returnValue = std::string(txt);
		ImGui::SetCursorPosX(GUI_INDENTATION_SIZE);
		ImGui::Text("%s", returnValue.c_str());
		va_end(args);
		delete[] txt;
	}

	void GUI::PlaceTextCenterX(const char* fmt, ...)
	{
		va_list args;
		va_start(args, fmt);
		char* txt = new char[1024];
		vsnprintf(txt, 1024, fmt, args);
		std::string returnValue = std::string(txt);
		ImGui::SetCursorPosX((ImGui::GetWindowSize().x - GetTextSizeX(returnValue)) * 0.5f);
		ImGui::Text("%s", returnValue.c_str());
		va_end(args);
		delete[] txt;
	}

	void GUI::PlaceTextRightX(const char* fmt, ...)
	{
		va_list args;
		va_start(args, fmt);
		char* txt = new char[1024];
		vsnprintf(txt, 1024, fmt, args);
		std::string returnValue = std::string(txt);
		ImGui::SetCursorPosX(ImGui::GetWindowSize().x - GetTextSizeX(returnValue) - GUI_INDENTATION_SIZE);
		ImGui::Text("%s", returnValue.c_str());
		va_end(args);
		delete[] txt;
	}

	void GUI::PlaceTextCenter(const char* fmt, ...)
	{
		va_list args;
		va_start(args, fmt);
		char* txt = new char[1024];
		vsnprintf(txt, 1024, fmt, args);
		std::string returnValue = std::string(txt);
		ImGui::SetCursorPosX((ImGui::GetWindowSize().x - GetTextSizeX(returnValue)) * 0.5f);
		ImGui::SetCursorPosY((ImGui::GetWindowSize().y - GetTextSizeY(returnValue)) * 0.5f);
		ImGui::Text("%s", returnValue.c_str());
		va_end(args);
		delete[] txt;
	}

	//void GUI::PlaceTextPercentage(float positionX, float positionY, const char* fmt, ...)
	//{
	//	float sizePixX = (maxX - minX) * sizeX;
	//	float sizePixY = (maxY - minY) * sizeY;
	//
	//	ImGui::SetNextWindowSize(ImVec2(sizePixX, sizePixY));
	//	ImGui::SetNextWindowPos(ImVec2(((maxX - minX) * positionX) + minX - (sizePixX / 2.0f), ((maxY - minY) * positionY) + minY - (sizePixY / 2.0f)));
	//}

	void GUI::SameLine()
	{
		ImGui::SameLine();
	}

	void GUI::PlaceTextAtPercentage(float percentageX, float percentageY, const char* fmt, ...)
	{
		va_list args;
		va_start(args, fmt);
		char* txt = new char[1024];
		vsnprintf(txt, 1024, fmt, args);
		std::string returnValue = std::string(txt);
		ImGui::SetCursorPosX((ImGui::GetWindowSize().x - GetTextSizeX(returnValue)) * percentageX);
		ImGui::SetCursorPosY((ImGui::GetWindowSize().y - GetTextSizeY(returnValue)) * percentageY);
		ImGui::Text("%s", returnValue.c_str());
		va_end(args);
		delete[] txt;
	}

	void GUI::MoveCursorToPercentage(float percentageX, float percentageY, float sizeX, float sizeY)
	{
		ImGui::SetCursorPosX((ImGui::GetWindowSize().x - sizeX) * percentageX);
		ImGui::SetCursorPosY((ImGui::GetWindowSize().y - sizeY) * percentageY);
	}

	void GUI::PlaceWindow(float positionX, float positionY, float sizeX, float sizeY, float minX, float minY, float maxX, float maxY)
	{
		float sizePixX = (maxX - minX) * sizeX;
		float sizePixY = (maxY - minY) * sizeY;
		
		ImGui::SetNextWindowSize(ImVec2(sizePixX, sizePixY));
		ImGui::SetNextWindowPos(ImVec2(((maxX - minX) * positionX) + minX, ((maxY - minY) * positionY) + minY));
	}

	void GUI::PlaceWindowCenter(float positionX, float positionY, float sizeX, float sizeY, float minX, float minY, float maxX, float maxY)
	{
		float sizePixX = (maxX - minX) * sizeX;
		float sizePixY = (maxY - minY) * sizeY;

		ImGui::SetNextWindowSize(ImVec2(sizePixX, sizePixY));
		ImGui::SetNextWindowPos(ImVec2(((maxX - minX) * positionX) + minX - (sizePixX / 2.0f), ((maxY - minY) * positionY) + minY - (sizePixY / 2.0f)));
	}

	void GUI::SetDarkTheme()
	{
		ImGuiStyle& style = ImGui::GetStyle();

		style.WindowRounding = 0.0f;
		style.ChildRounding = 0.0f;
		style.FrameRounding = 0.0f;
		style.GrabRounding = 0.0f;
		style.PopupRounding = 0.0f;
		style.ScrollbarRounding = 0.0f;
		style.WindowTitleAlign = ImVec2(0.5f, 0.5f);

		style.DisplayWindowPadding = ImVec2(19.0f, 19.0f);
		style.FramePadding = ImVec2(4.0f, 3.0f);
		style.FrameRounding = 0.0f;
		style.WindowPadding = ImVec2(8.0f, 8.0f);
		style.IndentSpacing = 21.0f;
		style.ItemInnerSpacing = ImVec2(4.0f, 4.0f);
		style.ItemSpacing = ImVec2(8.0f, 4.0f);

		//
		//style.ItemSpacing = ImVec2(0.0f, 0.0f);
		//style.ItemInnerSpacing = ImVec2(0.0f, 0.0f);
		//style.IndentSpacing = 0.0f;
		//

		style.Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
		style.Colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.06f, 0.06f, 0.94f);
		style.Colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		style.Colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
		style.Colors[ImGuiCol_Border] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
		style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		style.Colors[ImGuiCol_FrameBg] = ImVec4(0.16f, 0.29f, 0.48f, 0.54f);
		style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
		style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
		style.Colors[ImGuiCol_TitleBg] = ImVec4(0.16f, 0.29f, 0.48f, 1.00f);
		style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.16f, 0.29f, 0.48f, 1.00f);
		style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.16f, 0.29f, 0.48f, 1.00f);
		//style.Colors[ImGuiCol_TitleBg] = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
		//style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.16f, 0.29f, 0.48f, 1.00f);
		//style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
		style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
		style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
		style.Colors[ImGuiCol_CheckMark] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.24f, 0.52f, 0.88f, 1.00f);
		style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		style.Colors[ImGuiCol_Button] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
		style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.06f, 0.53f, 0.98f, 1.00f);
		style.Colors[ImGuiCol_Header] = ImVec4(0.26f, 0.59f, 0.98f, 0.31f);
		style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
		style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		style.Colors[ImGuiCol_Separator] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
		style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.10f, 0.40f, 0.75f, 0.78f);
		style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.10f, 0.40f, 0.75f, 1.00f);
		style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.59f, 0.98f, 0.25f);
		style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
		style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
		style.Colors[ImGuiCol_Tab] = ImVec4(0.18f, 0.35f, 0.58f, 0.862f);
		style.Colors[ImGuiCol_TabHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
		style.Colors[ImGuiCol_TabActive] = ImVec4(0.2f, 0.41f, 0.68f, 1.0f);
		style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.068f, 0.102f, 0.148, 0.9724f);
		style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.136f, 0.262f, 0.424f, 1.0f);
		style.Colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
		style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
		style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
		style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
		style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
		style.Colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
		style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
		style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
		style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
	}

	void GUI::SetLightTheme()
	{
		ImGuiStyle& style = ImGui::GetStyle();

		style.WindowRounding = 0.0f;
		style.ChildRounding = 0.0f;
		style.FrameRounding = 0.0f;
		style.GrabRounding = 0.0f;
		style.PopupRounding = 0.0f;
		style.ScrollbarRounding = 0.0f;
		style.WindowTitleAlign = ImVec2(0.5f, 0.5f);

		style.DisplayWindowPadding = ImVec2(19.0f, 19.0f);
		style.FramePadding = ImVec2(4.0f, 3.0f);
		style.FrameRounding = 0.0f;
		style.WindowPadding = ImVec2(8.0f, 8.0f);
		style.IndentSpacing = 21.0f;
		style.ItemInnerSpacing = ImVec2(4.0f, 4.0f);
		style.ItemSpacing = ImVec2(8.0f, 4.0f);

		style.Colors[ImGuiCol_Text] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
		style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
		//style.Colors[ImGuiCol_WindowBg] = ImVec4(0.94f, 0.94f, 0.94f, 1.00f);
		style.Colors[ImGuiCol_WindowBg] = ImVec4(0.94f, 0.94f, 0.94f, 0.6f);
		style.Colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		//style.Colors[ImGuiCol_PopupBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.98f);
		style.Colors[ImGuiCol_PopupBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.6f);
		style.Colors[ImGuiCol_Border] = ImVec4(0.00f, 0.00f, 0.00f, 0.30f);
		style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		style.Colors[ImGuiCol_FrameBg] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
		style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
		style.Colors[ImGuiCol_TitleBg] = ImVec4(0.82f, 0.82f, 0.82f, 1.00f);
		style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.82f, 0.82f, 0.82f, 1.00f);
		style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.82f, 0.82f, 0.82f, 1.00f);
		//style.Colors[ImGuiCol_TitleBg] = ImVec4(0.96f, 0.96f, 0.96f, 1.00f);
		//style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.82f, 0.82f, 0.82f, 1.00f);
		//style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 1.00f, 1.00f, 0.51f);
		style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.86f, 0.86f, 0.86f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.98f, 0.98f, 0.98f, 0.53f);
		style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.69f, 0.69f, 0.69f, 0.80f);
		style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.49f, 0.49f, 0.49f, 0.80f);
		style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.49f, 0.49f, 0.49f, 1.00f);
		style.Colors[ImGuiCol_CheckMark] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.26f, 0.59f, 0.98f, 0.78f);
		style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.46f, 0.54f, 0.80f, 0.60f);
		style.Colors[ImGuiCol_Button] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
		style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.06f, 0.53f, 0.98f, 1.00f);
		style.Colors[ImGuiCol_Header] = ImVec4(0.26f, 0.59f, 0.98f, 0.31f);
		style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
		style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		style.Colors[ImGuiCol_Separator] = ImVec4(0.39f, 0.39f, 0.39f, 0.62f);
		style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.14f, 0.44f, 0.80f, 0.78f);
		style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.14f, 0.44f, 0.80f, 1.00f);
		style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.80f, 0.80f, 0.80f, 0.56f);
		style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
		style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
		style.Colors[ImGuiCol_Tab] = ImVec4(0.764f, 0.797f, 0.964f, 0.379f);
		style.Colors[ImGuiCol_TabHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
		style.Colors[ImGuiCol_TabActive] = ImVec4(0.596f, 0.728f, 0.884f, 1.0f);
		style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.8088f, 0.9274f, 0.9608f, 0.1558f);
		style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.7416f, 0.8208f, 0.9144f, 1.0f);
		style.Colors[ImGuiCol_PlotLines] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
		style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
		style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
		style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.45f, 0.00f, 1.00f);
		style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
		style.Colors[ImGuiCol_DragDropTarget] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
		style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
		style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(0.70f, 0.70f, 0.70f, 0.70f);
		style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.20f);
		style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);
	}

	void GUI::SetEmptyTheme()
	{
		ImGuiStyle& style = ImGui::GetStyle();

		style.DisplayWindowPadding = ImVec2(0.0f, 0.0f);
		style.FramePadding = ImVec2(0.0f, 0.0f);
		style.FrameRounding = 0.0f;
		style.WindowPadding = ImVec2(0.0f, 0.0f);
		style.IndentSpacing = 0.0f;
		style.ItemInnerSpacing = ImVec2(0.0f, 0.0f);
		style.ItemSpacing = ImVec2(0.0f, 0.0f);

		style.Colors[ImGuiCol_Text] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
		style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
		style.Colors[ImGuiCol_WindowBg] = ImVec4(0.94f, 0.94f, 0.94f, 0.0f);
		style.Colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		style.Colors[ImGuiCol_PopupBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.6f);
		style.Colors[ImGuiCol_Border] = ImVec4(0.00f, 0.00f, 0.00f, 0.30f);
		style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		style.Colors[ImGuiCol_FrameBg] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
		style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
		style.Colors[ImGuiCol_TitleBg] = ImVec4(0.82f, 0.82f, 0.82f, 1.00f);
		style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.82f, 0.82f, 0.82f, 1.00f);
		style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.82f, 0.82f, 0.82f, 1.00f);
		style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.86f, 0.86f, 0.86f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.98f, 0.98f, 0.98f, 0.53f);
		style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.69f, 0.69f, 0.69f, 0.80f);
		style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.49f, 0.49f, 0.49f, 0.80f);
		style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.49f, 0.49f, 0.49f, 1.00f);
		style.Colors[ImGuiCol_CheckMark] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.26f, 0.59f, 0.98f, 0.78f);
		style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.46f, 0.54f, 0.80f, 0.60f);
		style.Colors[ImGuiCol_Button] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
		style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.06f, 0.53f, 0.98f, 1.00f);
		style.Colors[ImGuiCol_Header] = ImVec4(0.26f, 0.59f, 0.98f, 0.31f);
		style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
		style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		style.Colors[ImGuiCol_Separator] = ImVec4(0.39f, 0.39f, 0.39f, 0.62f);
		style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.14f, 0.44f, 0.80f, 0.78f);
		style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.14f, 0.44f, 0.80f, 1.00f);
		style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.80f, 0.80f, 0.80f, 0.56f);
		style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
		style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
		style.Colors[ImGuiCol_Tab] = ImVec4(0.764f, 0.797f, 0.964f, 0.379f);
		style.Colors[ImGuiCol_TabHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
		style.Colors[ImGuiCol_TabActive] = ImVec4(0.596f, 0.728f, 0.884f, 1.0f);
		style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.8088f, 0.9274f, 0.9608f, 0.1558f);
		style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.7416f, 0.8208f, 0.9144f, 1.0f);
		style.Colors[ImGuiCol_PlotLines] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
		style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
		style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
		style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.45f, 0.00f, 1.00f);
		style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
		style.Colors[ImGuiCol_DragDropTarget] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
		style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
		style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(0.70f, 0.70f, 0.70f, 0.70f);
		style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.20f);
		style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);
	}

	float GUI::GetTextSizeX(const std::string& text)
	{
		return ImGui::CalcTextSize(text.c_str()).x;
	}

	float GUI::GetTextSizeY(const std::string& text)
	{
		return ImGui::CalcTextSize(text.c_str()).y;
	}
}
