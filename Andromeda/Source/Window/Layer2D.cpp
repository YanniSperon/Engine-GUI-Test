#include "Layer2D.h"
#include "Window.h"
#include "Console.h"
#include "GUI.h"

#include <glew.h>

#define DARK_MODE

namespace AD {

	Layer2D::Layer2D(Layer2DType layer2DType, int width, int height, int x, int y)
		: Layer(LayerType::TWOD, width, height, x, y, "2D Layer"), m_OverlayOf(nullptr), m_Layer2DType(layer2DType)
	{
#ifdef DARK_MODE
		GUI::SetDarkTheme();
#else
		GUI::SetLightTheme();
#endif
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

	void Layer2D::DispatchWindowResize(int width, int height)
	{
		Layer::DispatchWindowResize(width, height);
		GUI::GetInstance()->Resize(width, height);
	}

	void Layer2D::Update(float deltaTime)
	{

	}

	void Layer2D::Draw()
	{
		static ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove/* | ImGuiWindowFlags_NoCollapse*/;

		glViewport(m_XPos, m_YPos, m_Width, m_Height);
		GUI::GetInstance()->Prepare();

		ImGuiIO* io = GUI::GetInstance()->GetIO();

		switch (m_Layer2DType)
		{
		case AD::Layer2DType::GUI:
		{
			ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
			GUI::PlaceWindowCenter(0.0f, 0.0f, 1.0f, 1.0f, m_XPos, m_YPos, m_XPos + m_Width, m_YPos + m_Height);
			ImGui::Begin("GUI##", nullptr, windowFlags);

			GUI::PlaceTextLeftX("%.1f FPS", io->Framerate);

			ImGui::End();
			ImGui::PopStyleColor();
			break;
		}
		case AD::Layer2DType::MENU:
		{
			GUI::PlaceWindowCenter(0.0f, 0.0f, 1.0f, 1.0f, m_XPos, m_YPos, m_XPos + m_Width, m_YPos + m_Height);
			ImGui::Begin("Menu##", nullptr, windowFlags);

			GUI::PlaceTextCenter("Paused");

			ImGui::End();
			break;
		}
		case AD::Layer2DType::EDITOR:
		{
			Scene& scene = m_OverlayOf->GetScene();
			GUI::PlaceWindowCenter(0.125f, 0.5f, 0.25f, 1.0f, m_XPos, m_YPos, m_XPos + m_Width, m_YPos + m_Height);
			ImGui::Begin("Scene##Left", nullptr, windowFlags);

			ImGui::BeginChild("Scene Graph##Left", ImVec2(0.0f, ImGui::GetWindowSize().y * 0.45f), false, ImGuiWindowFlags_HorizontalScrollbar);
			static Node* clipboardNode = nullptr;
			static Node* selectedNode = nullptr;
			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanFullWidth;
			if (!selectedNode) {
				flags |= ImGuiTreeNodeFlags_Selected;
			}
			bool rootNode = ImGui::TreeNodeEx((void*)(&scene), flags, "Scene - Nodes: %i", scene.GetNumNodes());
			if (ImGui::IsItemClicked()) {
				selectedNode = nullptr;
			}
			if (rootNode)
			{
				selectedNode = DrawSceneGraphRecursive(scene.GetRootNode(), selectedNode, false);
				ImGui::TreePop();
			}
			if (selectedNode && ImGui::IsWindowFocused()) {
				//if (io->KeysDown[AD_KEY_LEFT_CONTROL] && io->KeysDown[AD_KEY_C]) {
				//	clipboardNode = selectedNode;
				//}
				//if (clipboardNode && io->KeysDown[AD_KEY_LEFT_CONTROL] && io->KeysDown[AD_KEY_V]) {
				//	selectedNode = Node::DeepCopyTo(clipboardNode, selectedNode);
				//}
				if (m_Input.GetKeyboardKeyHeld(AD_KEY_LEFT_CONTROL) && m_Input.GetKeyboardKeyPressed(AD_KEY_C)) {
					clipboardNode = selectedNode;
				}
				if (clipboardNode && m_Input.GetKeyboardKeyHeld(AD_KEY_LEFT_CONTROL) && m_Input.GetKeyboardKeyPressed(AD_KEY_V)) {
					selectedNode = Node::DeepCopyTo(clipboardNode, selectedNode);
					clipboardNode = nullptr;
				}
			}
			ImGui::EndChild();
			ImGui::Separator();
			ImGui::BeginChild("Scene Attribute Inspector##Left");
			ImGui::Text("Scene attribs");
			ImGui::EndChild();
			ImGui::End();



			GUI::PlaceWindowCenter(0.875f, 0.5f, 0.25f, 1.0f, m_XPos, m_YPos, m_XPos + m_Width, m_YPos + m_Height);
			ImGui::Begin("Attribute Inspector##Right", nullptr, windowFlags);
			if (selectedNode) {
				ImGui::BeginChild("Node##Right", ImVec2(0.0f, ImGui::GetWindowSize().y * 0.20f));
				ImGui::Text("Node");
				ImGui::Indent();
				if (scene.GetRootNode() != selectedNode) {
					if (ImGui::Button("Delete Node##RightNode")) {
						Node* tempParent = selectedNode->GetParent();
						tempParent->DeleteChild(selectedNode);
						selectedNode = tempParent;
					}
				}
				if (ImGui::Button("Add Child##RightNode")) {
					selectedNode->CreateChild();
				}
				ImGui::EndChild();

				ImGui::Separator();

				ImGui::BeginChild("Object##Right", ImVec2(0.0f, ImGui::GetWindowSize().y * 0.20f));
				ImGui::Text("Object");
				ImGui::Indent();
				if (selectedNode->GetHasObjectComponent()) {
					if (ImGui::Button("Remove##RightObject")) {
						selectedNode->RemoveObjectComponent();
					}
				}
				else {
					if (ImGui::Button("Add##RightObject")) {
						selectedNode->AddObjectComponent(new Object("Resources/Cube.obj", "Resources/Basic", "Resources/Basic.png", false));
					}
				}
				ImGui::EndChild();

				ImGui::Separator();

				ImGui::BeginChild("Camera##Right", ImVec2(0.0f, ImGui::GetWindowSize().y * 0.20f));
				ImGui::Text("Camera");
				ImGui::Indent();
				if (selectedNode->GetHasCameraComponent()) {
					if (scene.GetRootNode() != selectedNode) {
						if (ImGui::Button("Remove##RightCamera")) {
							selectedNode->RemoveCameraComponent();
						}
					}
				}
				else {
					if (ImGui::Button("Add##RightCamera")) {
						selectedNode->AddCameraComponent(new Camera());
					}
				}
				ImGui::EndChild();

				ImGui::Separator();

				ImGui::BeginChild("Light##Right", ImVec2(0.0f, ImGui::GetWindowSize().y * 0.20f));
				ImGui::Text("Light");
				ImGui::Indent();
				if (selectedNode->GetHasLightComponent()) {
					if (ImGui::Button("Remove##RightLight")) {
						selectedNode->RemoveLightComponent();
					}
				}
				else {
					if (ImGui::Button("Add##RightLight")) {
						selectedNode->AddLightComponent(new Light());
					}
				}
				ImGui::EndChild();
			}


			ImGui::End();
			break;
		}
		default:
		{
			break;
		}
		}

		GUI::GetInstance()->Flush();
	}

	void Layer2D::SetOverlayOf(Layer3D* layer)
	{
		m_OverlayOf = layer;
	}

	Node* Layer2D::DrawSceneGraphRecursive(Node* node, Node* previouslySelectedNode, bool shouldUnselect)
	{
		Node* selectedNode = nullptr;
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanFullWidth;
		if (node == previouslySelectedNode && !shouldUnselect) {
			flags |= ImGuiTreeNodeFlags_Selected;
			selectedNode = previouslySelectedNode;
		}
		if (node->GetChildren().size() == 0) {
			flags |= ImGuiTreeNodeFlags_Leaf;
			flags |= ImGuiTreeNodeFlags_NoTreePushOnOpen;
			ImGui::TreeNodeEx((void*)node, flags, "%s - ID: %i", node->GetName().c_str(), node->GetID());
			if (ImGui::IsItemClicked()) {
				selectedNode = node;
			}
		}
		else {
			flags |= ImGuiTreeNodeFlags_OpenOnDoubleClick;
			flags |= ImGuiTreeNodeFlags_DefaultOpen;
			bool rootNode = ImGui::TreeNodeEx((void*)node, flags, "%s - ID: %i", node->GetName().c_str(), node->GetID());
			if (ImGui::IsItemClicked()) {
				selectedNode = node;
			}
			if (rootNode)
			{
				for (int i = 0; i < node->GetChildren().size(); i++) {
					Node* temp = DrawSceneGraphRecursive(node->GetChildren()[i], previouslySelectedNode, selectedNode);
					if (temp) {
						selectedNode = temp;
					}
				}

				ImGui::TreePop();
			}
		}
		return selectedNode;
	}
}



