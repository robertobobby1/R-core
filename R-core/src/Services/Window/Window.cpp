#include "rcpch.h"

//#include "GLFW/glfw3.h"
#include "glad/glad.h"

#include "Core/Core.h"
#include "Services/Window/Window.h"
#include "Services/GLFW/GLFWService.h"

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

		this->m_dependencies.push_back(
			DependencyDescriber("GLFW", GLFWService::Create(), false)
		);
	}

	void Window::Init()
	{
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
		m_Window = glfwCreateWindow((int)this->m_width, (int)this->m_height, this->m_title, nullptr, nullptr);

		glfwMakeContextCurrent(m_Window);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

		RC_ASSERT_MSG(status, "Failed to initialize Glad!");
	}

	void Window::OnUpdate()
	{
	}
}
