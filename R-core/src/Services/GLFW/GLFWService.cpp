#include "rcpch.h"

#include "Services/GLFW/GLFWService.h"
#include "Core/Core.h"

#include "GLFW/glfw3.h"


namespace RC {

	static void OnGLFWError(int error, const char* description) 
	{
		RC_LOG_ERROR("GLFW Error ({0}): {1}", error, description);
	}

	void GLFWService::Init()
	{
		int success = glfwInit();
		RC_ASSERT_MSG(success, "Could not initialize GLFW!");
		glfwSetErrorCallback(OnGLFWError);
	}

	void GLFWService::OnUpdate()
	{
	}

	std::shared_ptr<GLFWService> GLFWService::Create()
	{
		return std::make_shared<GLFWService>();
	}
}