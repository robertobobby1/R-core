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
		this->m_width = input.Width;
		this->m_height = input.Height;
		this->m_title = input.Title;
		this->m_Vsync = true;

		this->m_dependencies.push_back(
			DependencyDescriber("GLFW", RCGlfw::Create(), false)
		);
	}

	void Window::Init()
	{
		// set openGL version
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

		m_window = glfwCreateWindow((int)this->m_width, (int)this->m_height, this->m_title, nullptr, nullptr);

		glfwMakeContextCurrent(m_window);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		RC_ASSERT_MSG(status, "Failed to initialize Glad!");

		RCGlfw::PrintOpenGLInfo();
	}

	void Window::OnUpdate()
	{
	}
}
