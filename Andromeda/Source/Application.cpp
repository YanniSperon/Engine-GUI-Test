#include "Global.h"
#include "Console.h"
#include "Window.h"

#include "Object.h"
#include "Camera.h"

using namespace AD;

int main() {
	Global::Initialize();

	Window* window = Window::GetInstance();

	window->AddLayer3D();
	window->GetLastLayer().SetGetsKeyboardInput(true);
	window->GetLastLayer().SetGetsMouseInput(true);
	window->GetLastLayer().SetShouldBlockKeyboardInput(true);
	window->GetLastLayer().SetShouldBlockMouseInput(true);

	//Object* temp = new Object("Resources/Cube.obj", "Resources/Basic", "Resources/Basic.png", false);
	//temp->SetTranslation(glm::vec3(0.0f, 0.0f, 5.0f));
	//Camera* cam = new Camera();
	//cam->SetIsFocused(true);

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
		//cam->Update(deltaTime);
		window->Update(deltaTime);
		//temp->Draw(cam->GetProjectionMatrix(), cam->GetViewMatrix(), glm::mat4(1.0f));
		window->Draw();
		window->Flush();

		Global::Update();
	}

	Global::Cleanup();
}