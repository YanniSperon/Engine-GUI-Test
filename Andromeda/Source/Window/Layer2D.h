#pragma once

#include "Layer.h"
#include "Layer3D.h"

namespace AD {
	class Layer2D : public Layer {
	protected:
		Layer3D* m_OverlayOf;
	public:
		Layer2D(int width, int height, int x, int y);
		virtual ~Layer2D() override;

		virtual bool DispatchKeyboardInput(uint16_t key, uint16_t event) override;
		virtual bool DispatchMouseInput(uint16_t button, uint16_t event) override;
		virtual bool DispatchMouseMovement(double x, double y) override;
		virtual bool DispatchMouseScroll(double x, double y) override;
		virtual bool DispatchKeyTyped(unsigned int keycode) override;

		virtual void Update(float deltaTime) override;
		virtual void Draw() override;

		void SetOverlayOf(Layer3D* layer);
	};
}
