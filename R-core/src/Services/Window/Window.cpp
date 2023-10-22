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

	static void PrintOpenGLInfo()
	{
		GLint major, minor;
		glGetIntegerv(GL_MAJOR_VERSION, &major);
		glGetIntegerv(GL_MINOR_VERSION, &minor);

		RC_LOG_INFO("GL Vendor            : {0}", (char*)glGetString(GL_VENDOR));
		RC_LOG_INFO("GL Renderer          : {0}", (char*)glGetString(GL_RENDERER));
		RC_LOG_INFO("GL Version (string)  : {0}", (char*)glGetString(GL_VERSION));
		RC_LOG_INFO("GL Version (integer) : {0}.{1}", major, minor);
		RC_LOG_INFO("GLSL Version         : {0}", (char*)glGetString(GL_SHADING_LANGUAGE_VERSION));
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
		glfwSetErrorCallback(OnGLFWError);
		int success = glfwInit();
		RC_ASSERT_MSG(success, "Could not initialize GLFW!");

		// set openGL version
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

		m_Window = glfwCreateWindow((int)this->m_width, (int)this->m_height, this->m_title, nullptr, nullptr);

		glfwMakeContextCurrent(m_Window);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		RC_ASSERT_MSG(status, "Failed to initialize Glad!");
		PrintOpenGLInfo();
	}

	void Window::OnUpdate()
	{
	}
}
