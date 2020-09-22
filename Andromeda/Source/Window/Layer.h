#pragma once

#include "Input.h"

#include <cstdint>

namespace AD {
	enum class LayerType : uint16_t {
		TWOD = 0,
		THREED = 1
	};

	class Layer {
	private:
		LayerType m_Type;
	protected:
		Input m_Input;
		bool m_ShouldBlockMouseInput;
		bool m_ShouldBlockKeyboardInput;

		int m_Width;
		int m_Height;
		int m_XPos;
		int m_YPos;
	public:
		Layer(LayerType type, int width, int height, int x, int y);
		~Layer();

		void SetGetsKeyboardInput(bool doesWant);
		void SetGetsMouseInput(bool doesWant);

		void SetShouldBlockMouseInput(bool shouldBlock);
		void SetShouldBlockKeyboardInput(bool shouldBlock);

		bool DispatchKeyboardInput(uint16_t key, uint16_t event);
		bool DispatchMouseInput(uint16_t button, uint16_t event);
		void ProcessInput();
		virtual void Update(float deltaTime) = 0;
		virtual void Draw() = 0;
		void FlushInput();

		Input& GetInput();

		void SetWidth(int width);
		void SetHeight(int height);
		void SetXPos(int xPos);
		void SetYPos(int yPos);

		int GetWidth();
		int GetHeight();
		int GetXPos();
		int GetYPos();
	};
}
