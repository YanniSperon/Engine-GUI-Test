#include "Layer.h"

namespace AD {
	Layer::Layer(LayerType type, int width, int height, int x, int y, const std::string& debugName)
		: m_Type(type), m_Input(), m_ShouldBlockMouseInput(false), m_ShouldBlockKeyboardInput(false), m_Width(width), m_Height(height), m_XPos(x), m_YPos(y), m_DebugName(debugName), m_IsEnabled(true)
	{

	}

	Layer::~Layer()
	{

	}

	void Layer::SetShouldBlockMouseInput(bool shouldBlock)
	{
		m_ShouldBlockMouseInput = shouldBlock;
	}

	void Layer::SetShouldBlockKeyboardInput(bool shouldBlock)
	{
		m_ShouldBlockKeyboardInput = shouldBlock;
	}

	bool Layer::DispatchKeyboardInput(uint16_t key, uint16_t event)
	{
		if (m_IsEnabled) {
			m_Input.UpdateKeyboardKey(key, event);
			return !m_ShouldBlockKeyboardInput;
		}
		else {
			return true;
		}
	}

	bool Layer::DispatchMouseInput(uint16_t button, uint16_t event)
	{
		if (m_IsEnabled) {
			m_Input.UpdateMouseButton(button, event);
			return !m_ShouldBlockMouseInput;
		}
		else {
			return true;
		}
	}

	bool Layer::DispatchMouseMovement(double x, double y)
	{
		if (m_IsEnabled) {
			m_Input.MoveMouseTo(x, y);
			return !m_ShouldBlockMouseInput;
		}
		else {
			return true;
		}
	}

	bool Layer::DispatchMouseScroll(double x, double y)
	{
		if (m_IsEnabled) {
			m_Input.AddScrollPosition(x, y);
			return !m_ShouldBlockMouseInput;
		}
		else {
			return true;
		}
	}

	bool Layer::DispatchKeyTyped(unsigned int keycode)
	{
		if (m_IsEnabled) {
			return !m_ShouldBlockKeyboardInput;
		}
		else {
			return true;
		}
	}

	Input& Layer::GetInput()
	{
		return m_Input;
	}

	void Layer::SetWidth(int width)
	{
		m_Width = width;
	}

	void Layer::SetHeight(int height)
	{
		m_Height = height;
	}

	void Layer::SetXPos(int xPos)
	{
		m_XPos = xPos;
	}

	void Layer::SetYPos(int yPos)
	{
		m_YPos = yPos;
	}

	int Layer::GetWidth()
	{
		return m_Width;
	}

	int Layer::GetHeight()
	{
		return m_Height;
	}

	int Layer::GetXPos()
	{
		return m_XPos;
	}

	int Layer::GetYPos()
	{
		return m_YPos;
	}

	void Layer::SetDebugName(const std::string& name)
	{
		m_DebugName = name;
	}

	const std::string& Layer::GetDebugName()
	{
		return m_DebugName;
	}

	void Layer::EnableLayer()
	{
		m_IsEnabled = true;
	}

	void Layer::DisableLayer()
	{
		m_IsEnabled = false;
		m_Input.SetShouldCaptureKeyboardInput(false);
		m_Input.SetShouldCaptureKeyboardInput(true);
		m_Input.SetShouldCaptureMouseInput(false);
		m_Input.SetShouldCaptureMouseInput(true);
	}

}
