#include "rcpch.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "Core/Core.h"
#include "Services/Window/Window.h"
#include "Services/GLFW/GLFWService.h"

namespace RC {

	std::shared_ptr<Window> Window::Create(const WindowInput& input)
	{
		return std::make_shared<Window>(input);
	}

	static void OnGLFWError(int error, const char* description)
	{
		RC_LOG_ERROR("GLFW Error ({0}): {1}", error, description);
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
		int success = glfwInit();
		RC_ASSERT_MSG(success, "Could not initialize GLFW!");
		glfwSetErrorCallback(OnGLFWError);

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		m_Window = glfwCreateWindow((int)this->m_width, (int)this->m_height, this->m_title, nullptr, nullptr);

		glfwMakeContextCurrent(m_Window);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

		RC_ASSERT_MSG(status, "Failed to initialize Glad!");
	}

	void Window::OnUpdate()
	{
	}
}
