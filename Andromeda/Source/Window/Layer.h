#pragma once

#include "Input.h"

#include <cstdint>
#include <string>

namespace AD {
	enum class LayerType : uint16_t {
		TWOD = 0,
		THREED = 1,
		CUSTOM = 2
	};

	class Layer {
	protected:
		Input m_Input;
		bool m_ShouldBlockMouseInput;
		bool m_ShouldBlockKeyboardInput;
		bool m_IsEnabled;

		int m_Width;
		int m_Height;
		int m_XPos;
		int m_YPos;

		std::string m_DebugName;

		LayerType m_Type;
	public:
		Layer(LayerType type, int width, int height, int x, int y, const std::string& debugName);
		virtual ~Layer();

		void SetShouldBlockMouseInput(bool shouldBlock);
		void SetShouldBlockKeyboardInput(bool shouldBlock);

		virtual bool DispatchKeyboardInput(uint16_t key, uint16_t event);
		virtual bool DispatchMouseInput(uint16_t button, uint16_t event);
		virtual bool DispatchMouseMovement(double x, double y);
		virtual bool DispatchMouseScroll(double x, double y);
		virtual bool DispatchKeyTyped(unsigned int keycode);
		virtual void DispatchWindowResize(int width, int height);

		virtual void Update(float deltaTime) = 0;
		virtual void Draw() = 0;

		Input& GetInput();

		void SetWidth(int width);
		void SetHeight(int height);
		void SetXPos(int xPos);
		void SetYPos(int yPos);

		int GetWidth();
		int GetHeight();
		int GetXPos();
		int GetYPos();

		void SetDebugName(const std::string& name);
		const std::string& GetDebugName();

		void EnableLayer();
		void DisableLayer();
	};
}
