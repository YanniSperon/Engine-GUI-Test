#include "Global.h"
#include "Console.h"
#include "Window.h"

#include "Object.h"
#include "Camera.h"

using namespace AD;

int main() {
	Global::Initialize();

	Window* window = Window::GetInstance();

	Layer2D* overlay = window->AddLayer2D();
	Layer3D* layer = window->AddLayer3D();
	overlay->SetOverlayOf(layer);

	overlay->GetInput().SetShouldCaptureKeyboardInput(true);
	overlay->GetInput().SetShouldCaptureMouseInput(true);
	//overlay->SetShouldBlockKeyboardInput(true);
	//overlay->SetShouldBlockMouseInput(true);

	layer->GetInput().SetShouldCaptureKeyboardInput(true);
	layer->GetInput().SetShouldCaptureMouseInput(true);
	layer->SetShouldBlockKeyboardInput(true);
	layer->SetShouldBlockMouseInput(true);












	Node* tempNode = new Node();
	Object* temp = new Object("Resources/Cube.obj", "Resources/Basic", "Resources/Basic.png", false);
	temp->SetTranslation(glm::vec3(0.0f, 0.0f, 5.0f));
	tempNode->AddObjectComponent(temp);
	layer->GetScene().AddNode(tempNode);















	double timeConstant = 1.0;
	auto lastTime = std::chrono::high_resolution_clock::now();
	auto currentTime = lastTime;

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	while (!window->ShouldCloseWindow()) {

		currentTime = std::chrono::high_resolution_clock::now();
		auto deltaTimeNanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(currentTime - lastTime);
		lastTime = currentTime;
		double deltaTimeWithoutTimeFactor = deltaTimeNanoseconds.count() / 1000000000.0;
		double deltaTime = deltaTimeWithoutTimeFactor * timeConstant;

		window->Prepare();
		window->Update(deltaTime);
		window->Draw();
		window->Flush();

		Global::Update();
	}

	Global::Cleanup();
}