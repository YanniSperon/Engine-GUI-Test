#include "Window.h"
#include "Console.h"
#include "Config.h"

#include <glew.h>
#include <glfw3.h>

namespace AD {

	static Window* s_Window;

	static void GLAPIENTRY GLDebugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
	{
		switch (severity)
		{
		case GL_DEBUG_SEVERITY_HIGH:
			Console::FatalError("GL CALLBACK:%s type = 0x%x, severity = 0x%x, message = %s\n", (type == GL_DEBUG_TYPE_ERROR ? " ** GL ERROR **" : ""), type, severity, message);
			break;
		case GL_DEBUG_SEVERITY_MEDIUM:
			Console::Error("GL CALLBACK:%s type = 0x%x, severity = 0x%x, message = %s\n", (type == GL_DEBUG_TYPE_ERROR ? " ** GL ERROR **" : ""), type, severity, message);
			break;
		case GL_DEBUG_SEVERITY_LOW:
			Console::Warning("GL CALLBACK:%s type = 0x%x, severity = 0x%x, message = %s\n", (type == GL_DEBUG_TYPE_ERROR ? " ** GL ERROR **" : ""), type, severity, message);
			break;
		case GL_DEBUG_SEVERITY_NOTIFICATION:
			Console::Info("GL NOTIFICATION:%s type = 0x%x, severity = 0x%x, message = %s\n", (type == GL_DEBUG_TYPE_ERROR ? " ** GL ERROR **" : ""), type, severity, message);
			break;
		}
	}

	Window::Window(const std::string& name, int width, int height)
		: m_Width(width), m_Height(height), m_Window(nullptr), m_CursorXPos(0), m_CursorYPos(0), m_HasVSync(Config::GetInstance()->GetHasVSync()), m_Layers(), m_CurrentLayer(0), m_HasRawMouseMotion(Config::GetInstance()->GetHasRawMouseInput()), m_MouseMoved(false)
	{
		//////////////////////////////////////////////////////////////////////////////////////////////
		Console::Assert(glfwInit(), "Failed GLFW Initialization!");
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
		//////////////////////////////////////////////////////////////////////////////////////////////
		// replace first null with glfwGetPrimaryMonitor()
		m_Window = glfwCreateWindow(width, height, name.c_str(), NULL, NULL);
		Console::Assert(m_Window, "Failed Window Creation!");
		glfwMakeContextCurrent(m_Window);
		glfwSetWindowUserPointer(m_Window, this);
		//////////////////////////////////////////////////////////////////////////////////////////////
		GLenum err = glewInit();
		Console::Assert(err == GLEW_OK, "Failed GLEW Initialization - %s", reinterpret_cast<char const*>(glewGetErrorString(err)));
		//////////////////////////////////////////////////////////////////////////////////////////////
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_CCW);
		glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		//////////////////////////////////////////////////////////////////////////////////////////////
		if (glfwRawMouseMotionSupported() && m_HasRawMouseMotion) {
			glfwSetInputMode(m_Window, GLFW_RAW_MOUSE_MOTION, 1);
		}
		else {
			m_HasRawMouseMotion = false;
			Config::GetInstance()->SetHasRawMouseInput(false);
		}
		glfwSwapInterval(m_HasVSync);
		glDebugMessageCallback(GLDebugMessageCallback, 0);
		glfwSetErrorCallback([](int error, const char* description)
			{
				Console::Error("GLFW Error (%i): \"%s\"", error, description);
			}
		);
		//////////////////////////////////////////////////////////////////////////////////////////////
		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
			{
				Window& windowClass = *(Window*)glfwGetWindowUserPointer(window);
				windowClass.SetCursorX(xPos);
				windowClass.SetCursorY(yPos);
				windowClass.SetMouseMoved(true);
			}
		);
		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
			{
				Window& windowClass = *(Window*)glfwGetWindowUserPointer(window);

				switch (action)
				{
				case GLFW_PRESS:
				{
					windowClass.DispatchMouseInput(button, 1);
					break;
				}
				case GLFW_RELEASE:
				{
					windowClass.DispatchMouseInput(button, 3);
					break;
				}
				}
			}
		);
		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				Window& windowClass = *(Window*)glfwGetWindowUserPointer(window);

				switch (action)
				{
				case GLFW_PRESS:
				{
					windowClass.DispatchKeyboardInput(key, 1);
					break;
				}
				case GLFW_RELEASE:
				{
					windowClass.DispatchKeyboardInput(key, 3);
					break;
				}
				}
			}
		);
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
			{
				Window& windowClass = *(Window*)glfwGetWindowUserPointer(window);

				windowClass.SetWidth(width);
				windowClass.SetHeight(height);
			}
		);
	}

	Window::~Window()
	{
		glfwDestroyWindow(m_Window);
		glfwTerminate();
	}

	void Window::Prepare()
	{
		glfwPollEvents();
		for (int i = 0; i < m_Layers.size(); i++) {
			m_Layers[i]->ProcessInput();
		}
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void Window::DispatchMouseInput(uint16_t button, uint16_t event)
	{
		for (int i = 0; i < m_Layers.size(); i++) {
			if (!m_Layers[i]->DispatchMouseInput(button, event)) {
				break;
			}
		}
	}

	void Window::DispatchKeyboardInput(uint16_t button, uint16_t event)
	{
		for (int i = 0; i < m_Layers.size(); i++) {
			if (!m_Layers[i]->DispatchKeyboardInput(button, event)) {
				break;
			}
		}
	}

	void Window::Update(float deltaTime)
	{
		for (int i = 0; i < m_Layers.size(); i++) {
			m_CurrentLayer = i;
			m_Layers[i]->Update(deltaTime);
		}
		m_CurrentLayer = 0;
		m_MouseMoved = false;
	}

	void Window::Draw()
	{
		for (int i = m_Layers.size() - 1; i >= 0; i--) {
			m_CurrentLayer = i;
			m_Layers[i]->Draw();
		}
		m_CurrentLayer = 0;
		glfwSwapBuffers(m_Window);
	}

	void Window::Flush()
	{
		for (int i = 0; i < m_Layers.size(); i++) {
			m_Layers[i]->FlushInput();
		}
	}

	void Window::AddLayer2D()
	{
		m_Layers.push_back(std::move(std::make_unique<Layer2D>(m_Width, m_Height, 0, 0)));
	}

	void Window::AddLayer3D()
	{
		m_Layers.push_back(std::move(std::make_unique<Layer3D>(m_Width, m_Height, 0, 0)));
	}

	void Window::AddLayerCustom(LayerType type, const std::function<void(float)>& customUpdateCode, const std::function<void()>& customDrawCode)
	{
		m_Layers.push_back(std::move(std::make_unique<LayerCustom>(type, customUpdateCode, customDrawCode, m_Width, m_Height, 0, 0)));
	}

	int Window::GetCurrentLayerIndex()
	{
		return m_CurrentLayer;
	}

	int Window::GetWidth()
	{
		return m_Width;
	}

	int Window::GetHeight()
	{
		return m_Height;
	}

	bool Window::GetHasVSync()
	{
		return m_HasVSync;
	}

	double Window::GetCursorX()
	{
		return m_CursorXPos;
	}

	double Window::GetCursorY()
	{
		return m_CursorYPos;
	}

	bool Window::GetMouseMoved()
	{
		return m_MouseMoved;
	}

	void Window::SetVSync(bool vsync)
	{
		m_HasVSync = vsync;
		glfwSwapInterval(m_HasVSync);
	}

	void Window::SetCurrentLayerIndex(int index)
	{
		m_CurrentLayer = index;
	}

	void Window::IncrementCurrentLayer()
	{
		m_CurrentLayer++;
		if (m_CurrentLayer == m_Layers.size()) {
			m_CurrentLayer = 0;
		}
	}

	bool Window::EnableRawMouseMotion()
	{
		if (glfwRawMouseMotionSupported()) {
			glfwSetInputMode(m_Window, GLFW_RAW_MOUSE_MOTION, 1);
			return true;
		}
		else {
			return false;
		}
	}

	bool Window::ShouldCloseWindow()
	{
		return glfwWindowShouldClose(m_Window);
	}

	GLFWwindow* Window::GetGLFWwindow()
	{
		return m_Window;
	}

	Layer& Window::GetLayer(int index)
	{
		return *m_Layers[index];
	}

	Layer& Window::GetCurrentLayer()
	{
		return *m_Layers[m_CurrentLayer];
	}

	Layer& Window::GetLastLayer()
	{
		return *m_Layers[m_Layers.size() - 1];
	}

	void Window::InitializeSingleton()
	{
		s_Window = new Window("Andromeda", Config::GetInstance()->GetStartingWidth(), Config::GetInstance()->GetStartingHeight());
	}

	Window* Window::GetInstance()
	{
		return s_Window;
	}

	void Window::CleanupSingleton()
	{
		delete s_Window;
	}

	void Window::SetCursorX(double x)
	{
		m_CursorXPos = x;
	}

	void Window::SetCursorY(double y)
	{
		m_CursorYPos = y;
	}

	void Window::SetWidth(int x)
	{
		m_Width = x;
	}

	void Window::SetHeight(int y)
	{
		m_Height = y;
	}

	void Window::SetMouseMoved(bool didMove)
	{
		m_MouseMoved = didMove;
	}
}