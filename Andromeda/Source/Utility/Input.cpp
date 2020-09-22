#include "Input.h"
#include "Console.h"

namespace AD {

	Input::Input()
		: m_KeyboardKeyStates(512, 0), m_MouseButtonStates(16, 0), m_ShouldCaptureKeyboardInput(false), m_ShouldCaptureMouseInput(false), m_PendingMouseStateChanges(), m_PendingKeyboardStateChanges()
	{

	}

	Input::~Input()
	{

	}

	InputState Input::GetKeyboardKeyState(uint16_t key)
	{
		return static_cast<InputState>(m_KeyboardKeyStates[key]);
	}

	bool Input::GetKeyboardKeyIdle(uint16_t key)
	{
		return m_KeyboardKeyStates[key] == 0;
	}

	bool Input::GetKeyboardKeyPressed(uint16_t key)
	{
		return m_KeyboardKeyStates[key] == 1;
	}

	bool Input::GetKeyboardKeyHeld(uint16_t key)
	{
		return m_KeyboardKeyStates[key] == 2;
	}

	bool Input::GetKeyboardKeyReleased(uint16_t key)
	{
		return m_KeyboardKeyStates[key] == 3;
	}

	void Input::UpdateKeyboardKey(uint16_t key, uint16_t event)
	{
		if (m_ShouldCaptureKeyboardInput) {
			m_PendingKeyboardStateChanges.push_back(InputStateChange(key, event));
		}
	}

	InputState Input::GetMouseButtonState(uint16_t button)
	{
		return static_cast<InputState>(m_MouseButtonStates[button]);
	}

	bool Input::GetMouseButtonIdle(uint16_t button)
	{
		return m_MouseButtonStates[button] == 0;
	}

	bool Input::GetMouseButtonPressed(uint16_t button)
	{
		return m_MouseButtonStates[button] == 1;
	}

	bool Input::GetMouseButtonHeld(uint16_t button)
	{
		return m_MouseButtonStates[button] == 2;
	}

	bool Input::GetMouseButtonReleased(uint16_t button)
	{
		return m_MouseButtonStates[button] == 3;
	}

	void Input::UpdateMouseButton(uint16_t button, uint16_t event)
	{
		if (m_ShouldCaptureMouseInput) {
			m_PendingMouseStateChanges.push_back(InputStateChange(button, event));
		}
	}

	void Input::Prepare()
	{
		for (int i = 0; i < m_PendingKeyboardStateChanges.size(); i++) {
			m_KeyboardKeyStates[m_PendingKeyboardStateChanges[i].m_Button] = m_PendingKeyboardStateChanges[i].m_Type;
		}
		m_PendingKeyboardStateChanges.clear();
		for (int i = 0; i < m_PendingMouseStateChanges.size(); i++) {
			m_MouseButtonStates[m_PendingMouseStateChanges[i].m_Button] = m_PendingMouseStateChanges[i].m_Type;
		}
		m_PendingMouseStateChanges.clear();
	}

	void Input::Flush()
	{
		for (int i = 0; i < 512; i++) {
			if (m_KeyboardKeyStates[i] == 1) {
				m_KeyboardKeyStates[i] = 2;
			}
			else if (m_KeyboardKeyStates[i] == 3) {
				m_KeyboardKeyStates[i] = 0;
			}
		}
		for (int i = 0; i < 16; i++) {
			if (m_MouseButtonStates[i] == 1) {
				m_MouseButtonStates[i] = 2;
			}
			else if (m_MouseButtonStates[i] == 3) {
				m_MouseButtonStates[i] = 0;
			}
		}
	}

	void Input::StopCaptureMouseInput()
	{
		memset(&m_MouseButtonStates[0], 0, m_MouseButtonStates.size() * sizeof m_MouseButtonStates[0]);
		m_ShouldCaptureMouseInput = false;
	}

	void Input::StartCaptureMouseInput()
	{
		m_ShouldCaptureMouseInput = true;
	}

	void Input::StopCaptureKeyboardInput()
	{
		memset(&m_KeyboardKeyStates[0], 0, m_KeyboardKeyStates.size() * sizeof m_KeyboardKeyStates[0]);
		m_ShouldCaptureKeyboardInput = false;
	}

	void Input::StartCaptureKeyboardInput()
	{
		m_ShouldCaptureKeyboardInput = true;
	}
}
