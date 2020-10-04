#include "Global.h"
#include "Console.h"
#include "Window.h"

#include "Object.h"
#include "Camera.h"

using namespace AD;

int main() {
	Global::Initialize();

	Window* window = Window::GetInstance();

	Layer2D* overlay = window->AddLayer2D(Layer2DType::EDITOR);
	Layer3D* layer = window->AddLayer3D();
	overlay->SetOverlayOf(layer);

	overlay->GetInput().SetShouldCaptureKeyboardInput(true);
	overlay->GetInput().SetShouldCaptureMouseInput(true);
	overlay->SetShouldBlockKeyboardInput(true);
	overlay->SetShouldBlockMouseInput(true);

	layer->GetInput().SetShouldCaptureKeyboardInput(false);
	layer->GetInput().SetShouldCaptureMouseInput(false);
	layer->SetShouldBlockKeyboardInput(false);
	layer->SetShouldBlockMouseInput(false);

	window->ShowMouse();












	Node* tempNode = new Node();
	Object* temp = new Object("Resources/Cube.obj", "Resources/Basic", "Resources/Basic.png", false);
	temp->SetTranslation(glm::vec3(0.0f, 0.0f, 5.0f));
	tempNode->AddObjectComponent(temp);
	layer->GetScene().AddNode(tempNode);

	//Node* tempNode2 = new Node();
	//Object* temp2 = new Object("Resources/Sphere.obj", "Resources/Basic", "Resources/Basic.png", false);
	//temp2->SetTranslation(glm::vec3(0.0f, 0.0f, -5.0f));
	//tempNode2->AddObjectComponent(temp2);
	//layer->GetScene().AddNode(tempNode2);
	//
	//Node* tempNode3 = new Node();
	//Object* temp3 = new Object("Resources/Torus.obj", "Resources/Basic", "Resources/Basic.png", false);
	//temp3->SetTranslation(glm::vec3(5.0f, 0.0f, 0.0f));
	//tempNode3->AddObjectComponent(temp3);
	//layer->GetScene().AddNode(tempNode3);

	Node* tempNode4 = new Node();
	Object* temp4 = new Object("Resources/Plane.obj", "Resources/Basic", "Resources/White.png", false);
	temp4->SetTranslation(glm::vec3(0.0f, -5.0f, 0.0f));
	temp4->SetScale(glm::vec3(10.0f, 10.0f, 10.0f));
	tempNode4->AddObjectComponent(temp4);
	layer->GetScene().AddNode(tempNode4);















	double timeConstant = 1.0;
	auto lastTime = std::chrono::high_resolution_clock::now();
	auto currentTime = lastTime;

	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	//glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	while (!window->ShouldCloseWindow()) {

		currentTime = std::chrono::high_resolution_clock::now();
		auto deltaTimeNanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(currentTime - lastTime);
		lastTime = currentTime;
		double deltaTimeWithoutTimeFactor = deltaTimeNanoseconds.count() / 1000000000.0;
		double deltaTime = deltaTimeWithoutTimeFactor * timeConstant;
		window->Prepare();

		if (window->GetInput().GetKeyboardKeyPressed(AD_KEY_1)) {
			overlay->GetInput().SetShouldCaptureKeyboardInput(false);
			overlay->GetInput().SetShouldCaptureMouseInput(false);
			overlay->SetShouldBlockKeyboardInput(false);
			overlay->SetShouldBlockMouseInput(false);

			layer->GetInput().SetShouldCaptureKeyboardInput(true);
			layer->GetInput().SetShouldCaptureMouseInput(true);
			layer->SetShouldBlockKeyboardInput(true);
			layer->SetShouldBlockMouseInput(true);

			window->HideMouse();
		}

		if (window->GetInput().GetKeyboardKeyPressed(AD_KEY_2)) {
			overlay->GetInput().SetShouldCaptureKeyboardInput(true);
			overlay->GetInput().SetShouldCaptureMouseInput(true);
			overlay->SetShouldBlockKeyboardInput(true);
			overlay->SetShouldBlockMouseInput(true);

			layer->GetInput().SetShouldCaptureKeyboardInput(false);
			layer->GetInput().SetShouldCaptureMouseInput(false);
			layer->SetShouldBlockKeyboardInput(false);
			layer->SetShouldBlockMouseInput(false);

			window->ShowMouse();
		}

		window->Update(deltaTime);
		window->Draw();
		window->Flush();

		Global::Update();
	}

	Global::Cleanup();
}