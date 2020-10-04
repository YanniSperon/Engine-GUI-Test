#pragma once

#include "Layer.h"
#include "Layer3D.h"

namespace AD {
	enum class Layer2DType : uint16_t {
		GUI = 0,
		MENU = 1,
		EDITOR = 2
	};

	class Layer2D : public Layer {
	protected:
		Layer3D* m_OverlayOf;
		Layer2DType m_Layer2DType;
	public:
		Layer2D(Layer2DType layer2DType, int width, int height, int x, int y);
		virtual ~Layer2D() override;

		virtual bool DispatchKeyboardInput(uint16_t key, uint16_t event) override;
		virtual bool DispatchMouseInput(uint16_t button, uint16_t event) override;
		virtual bool DispatchMouseMovement(double x, double y) override;
		virtual bool DispatchMouseScroll(double x, double y) override;
		virtual bool DispatchKeyTyped(unsigned int keycode) override;
		virtual void DispatchWindowResize(int width, int height) override;

		virtual void Update(float deltaTime) override;
		virtual void Draw() override;

		void SetOverlayOf(Layer3D* layer);
	private:
		Node* DrawSceneGraphRecursive(Node* node, Node* previouslySelectedNode, bool shouldUnselect); // ImGuiTreeNodeFlags_Selected
	};
}
