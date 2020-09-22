#include "Layer.h"

namespace AD {
	Layer::Layer(LayerType type, int width, int height, int x, int y)
		: m_Type(type), m_Input(), m_ShouldBlockMouseInput(false), m_ShouldBlockKeyboardInput(false), m_Width(width), m_Height(height), m_XPos(x), m_YPos(y)
	{

	}

	Layer::~Layer()
	{

	}

	void Layer::SetGetsKeyboardInput(bool doesGet)
	{
		if (doesGet) {
			m_Input.StartCaptureKeyboardInput();
		}
		else {
			m_Input.StopCaptureKeyboardInput();
		}
	}

	void Layer::SetGetsMouseInput(bool doesGet)
	{
		if (doesGet) {
			m_Input.StartCaptureMouseInput();
		}
		else {
			m_Input.StopCaptureMouseInput();
		}
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
		m_Input.UpdateKeyboardKey(key, event);
		return !m_ShouldBlockKeyboardInput;
	}

	bool Layer::DispatchMouseInput(uint16_t button, uint16_t event)
	{
		m_Input.UpdateMouseButton(button, event);
		return !m_ShouldBlockMouseInput;
	}

	void Layer::ProcessInput()
	{
		m_Input.Prepare();
	}

	void Layer::FlushInput()
	{
		m_Input.Flush();
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

}
