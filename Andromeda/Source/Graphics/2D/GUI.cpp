#include "GUI.h"
#include "Input.h"
#include "Window.h"

#include <ImGuiOpenGLRenderer.h>

//#define ARIAL_FONT
//#define TIMES_NEW_ROMAN_FONT
#define TAHOMA_FONT

#define FONT_SIZE 16.0f

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
		static auto m_Font = m_IO->Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\arial.ttf", FONT_SIZE);
#elif defined(TAHOMA_FONT)
		static auto m_Font = m_IO->Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\tahoma.ttf", FONT_SIZE);
#elif defined(TIMES_NEW_ROMAN_FONT)
		static auto m_Font = m_IO->Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\times.ttf", FONT_SIZE);
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

	void GUI::PressButton(int button)
	{
		m_IO->MouseDown[button] = true;
	}

	void GUI::ReleaseButton(int button)
	{
		m_IO->MouseDown[button] = false;
	}

	void GUI::MoveMouse(float x, float y)
	{
		m_IO->MousePos = ImVec2(x, y);
	}

	void GUI::ScrollMouseWheel(float x, float y)
	{
		m_IO->MouseWheelH += x;
		m_IO->MouseWheel += y;
	}

	void GUI::PressKey(int key)
	{
		m_IO->KeysDown[key] = true;

		m_IO->KeyCtrl = m_IO->KeysDown[AD_KEY_LEFT_CONTROL] || m_IO->KeysDown[AD_KEY_RIGHT_CONTROL];
		m_IO->KeyShift = m_IO->KeysDown[AD_KEY_LEFT_SHIFT] || m_IO->KeysDown[AD_KEY_RIGHT_SHIFT];
		m_IO->KeyAlt = m_IO->KeysDown[AD_KEY_LEFT_ALT] || m_IO->KeysDown[AD_KEY_RIGHT_ALT];
		m_IO->KeySuper = m_IO->KeysDown[AD_KEY_LEFT_SUPER] || m_IO->KeysDown[AD_KEY_RIGHT_SUPER];
	}

	void GUI::ReleaseKey(int key)
	{
		m_IO->KeysDown[key] = false;
	}

	void GUI::TypeKey(int key)
	{
		if (key > 0 && key < 0x10000) {
			m_IO->AddInputCharacter((unsigned short)key);
		}
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
}
