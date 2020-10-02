#include "Layer2D.h"
#include "Window.h"
#include "Console.h"
#include "GUI.h"

#include <glew.h>

namespace AD {

	Layer2D::Layer2D(int width, int height, int x, int y)
		: Layer(LayerType::TWOD, width, height, x, y, "2D Layer"), m_OverlayOf(nullptr)
	{

	}

	Layer2D::~Layer2D()
	{

	}

	bool Layer2D::DispatchKeyboardInput(uint16_t key, uint16_t event)
	{
		if (m_IsEnabled && m_Input.GetShouldCaptureKeyboard()) {
			if (event == 1) {
				ImGuiIO* io = GUI::GetInstance()->GetIO();

				io->KeysDown[key] = true;

				io->KeyCtrl = io->KeysDown[AD_KEY_LEFT_CONTROL] || io->KeysDown[AD_KEY_RIGHT_CONTROL];
				io->KeyShift = io->KeysDown[AD_KEY_LEFT_SHIFT] || io->KeysDown[AD_KEY_RIGHT_SHIFT];
				io->KeyAlt = io->KeysDown[AD_KEY_LEFT_ALT] || io->KeysDown[AD_KEY_RIGHT_ALT];
				io->KeySuper = io->KeysDown[AD_KEY_LEFT_SUPER] || io->KeysDown[AD_KEY_RIGHT_SUPER];
			}
			else {
				GUI::GetInstance()->GetIO()->KeysDown[key] = false;
			}
		}

		return Layer::DispatchKeyboardInput(key, event);
	}

	bool Layer2D::DispatchMouseInput(uint16_t button, uint16_t event)
	{
		if (m_IsEnabled && m_Input.GetShouldCaptureMouse()) {
			if (event == 1) {
				GUI::GetInstance()->GetIO()->MouseDown[button] = true;
			}
			else {
				GUI::GetInstance()->GetIO()->MouseDown[button] = false;
			}
		}

		return Layer::DispatchMouseInput(button, event);
	}

	bool Layer2D::DispatchMouseMovement(double x, double y)
	{
		if (m_IsEnabled && m_Input.GetShouldCaptureMouse()) {
			GUI::GetInstance()->GetIO()->MousePos = ImVec2(x, y);
		}

		return Layer::DispatchMouseMovement(x, y);
	}

	bool Layer2D::DispatchMouseScroll(double x, double y)
	{
		if (m_IsEnabled && m_Input.GetShouldCaptureMouse()) {
			ImGuiIO* io = GUI::GetInstance()->GetIO();

			io->MouseWheelH += x;
			io->MouseWheel += y;
		}

		return Layer::DispatchMouseScroll(x, y);
	}

	bool Layer2D::DispatchKeyTyped(unsigned int keycode)
	{
		if (m_IsEnabled && m_Input.GetShouldCaptureKeyboard()) {
			if (keycode > 0 && keycode < 0x10000) {
				GUI::GetInstance()->GetIO()->AddInputCharacter((unsigned short)keycode);
			}
		}

		return Layer::DispatchKeyTyped(keycode);
	}

	void Layer2D::Update(float deltaTime)
	{
		
	}

	void Layer2D::Draw()
	{
		glViewport(m_XPos, m_YPos, m_Width, m_Height);
		GUI::GetInstance()->Prepare();

		//draw
		static bool show = true;
		ImGui::ShowDemoWindow(&show);

		GUI::GetInstance()->Flush();
	}

	void Layer2D::SetOverlayOf(Layer3D* layer)
	{
		m_OverlayOf = layer;
	}
}
