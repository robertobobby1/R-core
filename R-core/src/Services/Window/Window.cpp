#include "rcpch.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "Core/Core.h"
#include "Services/Window/Window.h"
#include "Services/GLFW/RCGlfw.h"

namespace RC {

	std::shared_ptr<Window> Window::Create(const WindowInput& input)
	{
		return std::make_shared<Window>(input);
	}

	Window::Window(const WindowInput& input) : Service()
	{
		this->m_data.m_height = input.Height;
		this->m_data.m_width  = input.Width;
		this->m_data.m_title  = input.Title;
		this->m_data.m_Vsync  = true;
		this->m_data.m_eventCallback = RC_BIND_FN(Service::CallDepCallbacks);

		this->AddDependencyCallback(RC_BIND_FN(Window::OnDispatchable));

		// Will initialize glfw environment
		this->m_glfw = RCGlfw::Create();
		this->m_dependencies.push_back(
			DependencyDescriber("GLFW", this->m_glfw, false)
		);
	}

	Window::~Window() 
	{
		glfwDestroyWindow(m_window);
		Service::~Service();
	}

	void Window::Init()
	{
		// set openGL version
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

		m_window = glfwCreateWindow(
			(int)m_data.m_width, (int)m_data.m_height, m_data.m_title, nullptr, nullptr
		);

		glfwMakeContextCurrent(m_window);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		RC_ASSERT_MSG(status, "Failed to initialize Glad!");

		this->m_glfw->PrintOpenGLInfo();

		glfwSetWindowUserPointer(m_window, &m_data);
		SetVSync(m_data.m_Vsync);

		// Set GLFW callbacks
		glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, int width, int height)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.m_width = width;
			data.m_height = height;

			// implement callback event
		});

		glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.m_eventCallback(OnWindowCloseEvent());
		});

		glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			// implement callback event
		});

		glfwSetCharCallback(m_window, [](GLFWwindow* window, unsigned int keycode)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			// implement callback event
		});

		glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			// implement callback event
		});

		glfwSetScrollCallback(m_window, [](GLFWwindow* window, double xOffset, double yOffset)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			// implement callback event
		});

		glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double xPos, double yPos)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			// implement callback event
		});
	}

	void Window::OnDispatchable(Dispatchable& dispatchable)
	{
		Dispatcher disp(dispatchable);
		disp.Dispatch<OnWindowCloseEvent>([this](Dispatchable& dispatchable) {
			this->~Window();
		});
	}

	void Window::SetVSync(bool _Vsync)
	{
		if (_Vsync)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		m_data.m_Vsync = _Vsync;
	}

	void Window::Run()
	{
	}
}
