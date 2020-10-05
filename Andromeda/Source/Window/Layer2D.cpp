#include "Layer2D.h"
#include "Window.h"
#include "Console.h"
#include "GUI.h"
#include "Mesh3DManager.h"
#include "TextureManager.h"
#include "ShaderManager.h"

#include <glew.h>

#define DARK_MODE

namespace AD {

	static Node* s_SelectedNode = nullptr;

	void DrawSceneGraphRecursive(Node* node)
	{
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanFullWidth;
		if (s_SelectedNode == node) {
			flags |= ImGuiTreeNodeFlags_Selected;
		}
		if (node->GetChildren().size() == 0) {
			flags |= ImGuiTreeNodeFlags_Leaf;
			flags |= ImGuiTreeNodeFlags_NoTreePushOnOpen;
			ImGui::TreeNodeEx((void*)node, flags, "%s - ID: %i", node->GetName().c_str(), node->GetID());
			if (ImGui::IsItemClicked()) {
				s_SelectedNode = node;
			}
		}
		else {
			flags |= ImGuiTreeNodeFlags_OpenOnDoubleClick;
			flags |= ImGuiTreeNodeFlags_DefaultOpen;
			bool rootNode = ImGui::TreeNodeEx((void*)node, flags, "%s - ID: %i", node->GetName().c_str(), node->GetID());
			if (ImGui::IsItemClicked()) {
				s_SelectedNode = node;
			}
			if (rootNode)
			{
				for (int i = 0; i < node->GetChildren().size(); i++) {
					DrawSceneGraphRecursive(node->GetChildren()[i]);
				}

				ImGui::TreePop();
			}
		}
	}

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
		static ImGuiWindowFlags windowFlags = /*ImGuiWindowFlags_NoResize |*/ ImGuiWindowFlags_NoMove/* | ImGuiWindowFlags_NoCollapse*/;

		glViewport(m_XPos, m_YPos, m_Width, m_Height);
		GUI::GetInstance()->Prepare();

		ImGuiIO* io = GUI::GetInstance()->GetIO();

#ifdef DARK_MODE
		GUI::SetDarkTheme();
#else
		GUI::SetLightTheme();
#endif
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
			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanFullWidth;
			if (!s_SelectedNode) {
				flags |= ImGuiTreeNodeFlags_Selected;
			}
			bool rootNode = ImGui::TreeNodeEx((void*)(&scene), flags, "Scene - Nodes: %i", scene.GetNumNodes());
			if (ImGui::IsItemClicked()) {
				s_SelectedNode = nullptr;
			}
			if (rootNode)
			{
				DrawSceneGraphRecursive(scene.GetRootNode());
				ImGui::TreePop();
			}
			if (s_SelectedNode && ImGui::IsWindowFocused()) {
				if (s_SelectedNode != scene.GetRootNode() && m_Input.GetKeyboardKeyPressed(AD_KEY_DELETE)) {
					Node* tempParent = s_SelectedNode->GetParent();
					tempParent->DeleteChild(s_SelectedNode);
					s_SelectedNode = tempParent;
				}
				if (m_Input.GetKeyboardKeyHeld(AD_KEY_LEFT_CONTROL) && m_Input.GetKeyboardKeyPressed(AD_KEY_C)) {
					clipboardNode = s_SelectedNode;
				}
				if (clipboardNode && clipboardNode != scene.GetRootNode() && m_Input.GetKeyboardKeyHeld(AD_KEY_LEFT_CONTROL) && m_Input.GetKeyboardKeyPressed(AD_KEY_V)) {
					s_SelectedNode = Node::DeepCopyTo(clipboardNode, s_SelectedNode);
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
			if (s_SelectedNode) {
				ImGui::BeginChild("Node##Right", ImVec2(0.0f, ImGui::GetWindowSize().y * 0.20f));
				ImGui::Text("Node");
				ImGui::Indent();
				ImGui::InputText("Name##NodeRight", s_SelectedNode->GetNameRef().data(), 64);
				if (ImGui::Button("Add Child##NodeRight")) {
					s_SelectedNode->CreateChild();
				}
				if (ImGui::TreeNodeEx("Position##NodeRight", ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanFullWidth))
				{
					ImGui::Text("Translation");
					ImGui::Indent();
					ImGui::InputFloat3("##NodeRightTranslation", &s_SelectedNode->GetTranslationRef()[0], 2);
					ImGui::Unindent();
					ImGui::Text("Rotation");
					ImGui::Indent();
					ImGui::InputFloat3("##NodeRightRotation", &s_SelectedNode->GetRotationRef()[0], 2);
					ImGui::Unindent();
					ImGui::TreePop();
				}
				if (scene.GetRootNode() != s_SelectedNode) {
					if (ImGui::Button("Delete Node##NodeRight")) {
						Node* tempParent = s_SelectedNode->GetParent();
						tempParent->DeleteChild(s_SelectedNode);
						s_SelectedNode = tempParent;
					}
				}
				ImGui::EndChild();

				ImGui::Separator();

				ImGui::BeginChild("Object##Right", ImVec2(0.0f, ImGui::GetWindowSize().y * 0.20f));
				ImGui::Text("Object");
				ImGui::Indent();
				if (s_SelectedNode->GetHasObjectComponent()) {
					if (ImGui::TreeNodeEx("Offset##ObjectRight", ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanFullWidth))
					{
						ImGui::Text("Translation");
						ImGui::Indent();
						ImGui::InputFloat3("##ObjectRightTranslation", &s_SelectedNode->GetObjectComponent()->GetTranslationRef()[0], 2);
						ImGui::Unindent();
						ImGui::Text("Rotation");
						ImGui::Indent();
						ImGui::InputFloat3("##ObjectRightRotation", &s_SelectedNode->GetObjectComponent()->GetRotationRef()[0], 2);
						ImGui::Unindent();
						ImGui::TreePop();
					}
					if (ImGui::Button("Remove##RightObject")) {
						s_SelectedNode->RemoveObjectComponent();
					}
				}
				else {
					if (ImGui::Button("Add##RightObject")) {
						s_SelectedNode->AddObjectComponent(new Object("Resources/Cube.obj", "Resources/Basic", "Resources/Basic.png", false));
					}
				}
				ImGui::EndChild();

				ImGui::Separator();

				ImGui::BeginChild("Camera##Right", ImVec2(0.0f, ImGui::GetWindowSize().y * 0.20f));
				ImGui::Text("Camera");
				ImGui::Indent();
				if (s_SelectedNode->GetHasCameraComponent()) {
					if (ImGui::TreeNodeEx("Offset##CameraRight", ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanFullWidth))
					{
						ImGui::Text("Translation");
						ImGui::Indent();
						ImGui::InputFloat3("##CameraRightTranslation", &s_SelectedNode->GetCameraComponent()->GetTranslationRef()[0], 2);
						ImGui::Unindent();
						ImGui::Text("Rotation");
						ImGui::Indent();
						ImGui::InputFloat3("##CameraRightRotation", &s_SelectedNode->GetCameraComponent()->GetRotationRef()[0], 2);
						ImGui::Unindent();
						ImGui::TreePop();
					}
					if (ImGui::TreeNodeEx("Properties##CameraRight", ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanFullWidth))
					{
						ImGui::Text("Near Plane");
						ImGui::Indent();
						ImGui::InputFloat("##CameraRightNearPlane", &s_SelectedNode->GetCameraComponent()->GetNearPlaneRef(), 0.01f, 0.1f, "%.2f");
						ImGui::Unindent();
						ImGui::Text("Far Plane");
						ImGui::Indent();
						ImGui::InputFloat("##CameraRightFarPlane", &s_SelectedNode->GetCameraComponent()->GetFarPlaneRef(), 0.1f, 1.0f, "%.2f");
						ImGui::Unindent();
						ImGui::Text("Movement Speed");
						ImGui::Indent();
						ImGui::InputFloat("##CameraRightMovementSpeed", &s_SelectedNode->GetCameraComponent()->GetMovementSpeedRef(), 0.5f, 5.0f, "%.2f");
						ImGui::Unindent();
						ImGui::TreePop();
					}
					if (scene.GetRootNode() != s_SelectedNode) {
						if (ImGui::Button("Remove##RightCamera")) {
							s_SelectedNode->RemoveCameraComponent();
						}
					}
				}
				else {
					if (ImGui::Button("Add##RightCamera")) {
						s_SelectedNode->AddCameraComponent(new Camera());
					}
				}
				ImGui::EndChild();

				ImGui::Separator();

				ImGui::BeginChild("Light##Right", ImVec2(0.0f, ImGui::GetWindowSize().y * 0.20f));
				ImGui::Text("Light");
				ImGui::Indent();
				if (s_SelectedNode->GetHasLightComponent()) {
					if (ImGui::TreeNodeEx("Offset##LightRight", ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanFullWidth))
					{
						ImGui::Text("Translation");
						ImGui::Indent();
						ImGui::InputFloat3("##LightRightTranslation", &s_SelectedNode->GetLightComponent()->GetTranslationRef()[0], 2);
						ImGui::Unindent();
						ImGui::Text("Rotation");
						ImGui::Indent();
						ImGui::InputFloat3("##LightRightRotation", &s_SelectedNode->GetLightComponent()->GetRotationRef()[0], 2);
						ImGui::Unindent();
						ImGui::TreePop();
					}
					if (ImGui::Button("Remove##RightLight")) {
						s_SelectedNode->RemoveLightComponent();
					}
				}
				else {
					if (ImGui::Button("Add##RightLight")) {
						s_SelectedNode->AddLightComponent(new Light());
					}
				}
				ImGui::EndChild();
			}
			else {
				ImGui::Text("Assets");
				ImGui::Indent();

				// static mesh manager
				std::unordered_map<std::string, std::shared_ptr<StaticMesh3D>>& staticMeshes = Mesh3DManager::GetInstance()->GetManagedStaticMeshes();
				static StaticMesh3D* selectedStaticMesh = nullptr;
				std::string selectedStaticName = "";
				if (selectedStaticMesh) {
					selectedStaticName = selectedStaticMesh->GetPath();
				}
				ImGui::Text("Static Meshes");
				ImGui::Indent();
				if (ImGui::BeginCombo("##RightSceneStatic", selectedStaticName.c_str()))
				{
					for (auto it = staticMeshes.begin(); it != staticMeshes.end(); it++)
					{
						if (ImGui::Selectable(it->first.c_str(), selectedStaticMesh == it->second.get())) {
							selectedStaticMesh = it->second.get();
						}
						if (selectedStaticMesh == it->second.get()) {
							ImGui::SetItemDefaultFocus();
						}
					}
					ImGui::EndCombo();
				}
				ImGui::Unindent();

				// dynamic mesh manager
				std::vector<std::shared_ptr<DynamicMesh3D>>& dynamicMeshes = Mesh3DManager::GetInstance()->GetManagedDynamicMeshes();
				static DynamicMesh3D* selectedDynamicMesh = nullptr;
				std::string selectedDynamicName = "";
				if (selectedDynamicMesh) {
					selectedDynamicName = selectedDynamicMesh->GetPath();
				}
				ImGui::Text("Dynamic Meshes");
				ImGui::Indent();
				if (ImGui::BeginCombo("##RightSceneDynamic", selectedDynamicName.c_str()))
				{
					for (int i = 0; i < dynamicMeshes.size(); i++) {
						if (ImGui::Selectable(dynamicMeshes[i]->GetPath().c_str(), selectedDynamicMesh == dynamicMeshes[i].get())) {
							selectedDynamicMesh = dynamicMeshes[i].get();
						}
						if (selectedDynamicMesh == dynamicMeshes[i].get()) {
							ImGui::SetItemDefaultFocus();
						}
					}
					ImGui::EndCombo();
				}
				ImGui::Unindent();

				// texture manager
				std::unordered_map<std::string, std::shared_ptr<Texture>>& textures = TextureManager::GetInstance()->GetManagedTextures();
				static Texture* selectedTexture = nullptr;
				std::string selectedTextureName = "";
				if (selectedTexture) {
					selectedTextureName = selectedTexture->GetPath();
				}
				ImGui::Text("Textures");
				ImGui::Indent();
				if (ImGui::BeginCombo("##RightSceneTexture", selectedTextureName.c_str()))
				{
					for (auto it = textures.begin(); it != textures.end(); it++)
					{
						if (ImGui::Selectable(it->first.c_str(), selectedTexture == it->second.get())) {
							selectedTexture = it->second.get();
						}
						if (selectedTexture == it->second.get()) {
							ImGui::SetItemDefaultFocus();
						}
					}
					ImGui::EndCombo();
				}
				ImGui::Unindent();
				// shader manager
				std::unordered_map<std::string, std::shared_ptr<Shader>>& shaders = ShaderManager::GetInstance()->GetManagedShaders();
				static Shader* selectedShader = nullptr;
				std::string selectedShaderName = "";
				if (selectedShader) {
					selectedShaderName = selectedShader->GetPath();
				}
				ImGui::Text("Shaders");
				ImGui::Indent();
				if (ImGui::BeginCombo("##RightSceneShaders", selectedShaderName.c_str()))
				{
					for (auto it = shaders.begin(); it != shaders.end(); it++)
					{
						if (ImGui::Selectable(it->first.c_str(), selectedShader == it->second.get())) {
							selectedShader = it->second.get();
						}
						if (selectedShader == it->second.get()) {
							ImGui::SetItemDefaultFocus();
						}
					}
					ImGui::EndCombo();
				}
				ImGui::Unindent();
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
}



